#!/bin/bash 

# Fill in these details for your application
appName="ci-plugin-demo"
bundleIdentifier="com.ADC.CiPluginDemo"
version=0.1.0

applicationCertificate="Developer ID Application: The Audio Programmer Ltd (FJY56HBNSL)"
installerCertificate="Developer ID Installer: The Audio Programmer Ltd (FJY56HBNSL)"
certificateIdentifier="FJY56HBNSL"

usage() { echo "Usage: $0 -u <email-address> -p <one-time-password> -v <version-number (0.1.0)>" 1>&2; exit 1; }

# Accept user input for both username and password
while getopts ":u:p:v:" o; do
    case "${o}" in
        u)
            username=${OPTARG}        
            ;;
        p)
            password=${OPTARG}
            ;;
        v)
            version=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done

# Ensure that both username and password are supplied
if [[ "$username" == "" || "$password" == "" ]]
then
    usage
fi

# Code sign the VST3 and AU files
codesign --force --deep --options runtime --sign "$applicationCertificate" ${appName}.vst3
codesign --force --deep --options runtime --sign "$applicationCertificate" ${appName}.component

# Run the command line tool for Whitebox Packages to build the pkg file
packagesbuild -v ${appName}.pkgproj

# Sign the pkg file with the developerId certificate
productsign --sign "$installerCertificate" build/${appName}.pkg build/${appName}-${version}.pkg

# Notarize the package. This uploads the package to Apple's notarization service. We need to wait for a response from the service
echo "Submitting package for notarization..."
xcrun altool --notarize-app --primary-bundle-id $bundleIdentifier --username $username --password $password --asc-provider $certificateIdentifier --file build/${appName}-${version}.pkg &> tempfile

# Query the notarization service every 2 minutes. If the notarization isn't complete yet, the script sleeps for 2 minutes
uuid=`cat tempfile | grep -Eo '\w{8}-(\w{4}-){3}\w{12}$'`
retryAttempt=0
numberOfRetries=10
intervalBetweenRetry=120

while [ $((retryAttempt++)) -lt $numberOfRetries ] 
do
    echo "Checking for notarization. Attempt $retryAttempt"
 
    # Check if notarization is complete for the request Id provided
    xcrun altool --notarization-info "$uuid" --username $username --password $password &> tempfile
    result=`cat tempfile`
    success=`echo "$result" | grep "success"`
    failure=`echo "$result" | grep "invalid"`
    if [[ "$success" != "" ]]; then
        echo "Notarization complete."

        # Notarization is successfully complete. Staple the certificate to the pkg file.
        xcrun stapler staple build/${appName}-${version}.pkg
        echo "Stapled the certificate."
        break
    fi
    if [[ "$failure" != "" ]]; then

        # Notarization failed for some reason. Breaking out.
        echo "$result"
        return 1
    fi
    echo "Notarization unfinished. Sleeping for $intervalBetweenRetry seconds before retrying."
    sleep $intervalBetweenRetry
done

rm build/${appName}.pkg
rm tempfile
rm -rf ${appName}.vst3
rm -rf ${appName}.component