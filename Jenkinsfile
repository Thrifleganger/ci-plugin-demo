/*
	This is a declarative pipeline script for Jenkins for Projucer type JUCE plugin projects
	Fill the variables at the start of this file to configure the build pipeline for your plugin.
	The build pipeline performs the following stages: 
	1) Checkout: The build pipeline is triggered for every change to the master branch. The Checkout stage clones the project locally within the Jenkins executor.
	2) Build: Depending on the OS, the corresponding build systems is utilized to build VST3 and AU targets.
	3) Test: The plugin is subjected to a testing application called pluginval. 
	4) Deploy: The build artifacts, including the VST3 and AU files are uploaded to an S3 bucket.
	5) Notify: A slack notification is sent out to the corresponding channel to notify whether a build has succeeded or failed.
*/

/* 
	The project name is the name of projucer file / plugin name configured within the projucer file. Avoid spaces.
*/
def projectName = 'ci-plugin-demo'

/*
	The clone URL of the git repo. Add the SSH variant which starts with "git@github.com"
*/
def gitRepoUrl = 'git@github.com:Thrifleganger/ci-plugin-demo.git'

def version = ''

/*
	Start of pipeline script. 
*/
pipeline {
    agent any
	triggers {
		pollSCM('*/2 * * * *')
	}	
    stages {
		stage('Checkout') {
			steps {	
				cleanWs()
				checkout([
					$class: 'GitSCM', 
					branches: [[name: '*/master']], 
					extensions: [], 
					userRemoteConfigs: [[
						credentialsId: 'jenkins-to-github', 
						url: "${gitRepoUrl}"
					]]
				])
			}
		}
        stage('Build') { 
            steps {
				script {				
					if (!isUnix()) {
						echo "Building plugin for Windows"
						bat """
							curl -L "https://api.juce.com/api/v1/download/juce/latest/windows" -o juce.zip
							jar xf juce.zip
							"JUCE/projucer.exe" --set-global-search-path windows defaultJuceModulePath JUCE/modules
							"JUCE/projucer.exe" --resave "${projectName}.jucer"													
							cd Builds/VisualStudio2019							
							if not defined MSBUILD_EXE set MSBUILD_EXE=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/MSBuild/Current/Bin/MSBuild.exe
							"%MSBUILD_EXE%" ${projectName}.sln /p:VisualStudioVersion=16.0 /m /p:Configuration=Release /p:Platform=x64 /p:PreferredToolArchitecture=x64			
							copy "x64\\Release\\VST3\\${projectName}.vst3" "..\\..\\Installer\\Windows\\${projectName}.vst3"							
						"""
						def versionScript = """
							@(
								"JUCE/projucer.exe" --get-version ${projectName}.jucer
							)
						"""
						version = bat(script: versionScript, returnStdout: true).trim()
					} else {
						echo "Building plugin for Mac"
						sh """
							curl -L https://api.juce.com/api/v1/download/juce/latest/osx -o juce.zip
							unzip juce
							./JUCE/Projucer.app/Contents/MacOS/Projucer --resave ${projectName}.jucer							
							cd Builds/MacOSX
							xcodebuild -configuration Release -scheme "${projectName} - All" build
							cp -R `readlink build/Release/${projectName}.vst3` ../../Installer/Mac/${projectName}.vst3
							cp -R `readlink build/Release/${projectName}.component` ../../Installer/Mac/${projectName}.component
						"""
						version = sh(
							script: "./JUCE/Projucer.app/Contents/MacOS/Projucer --get-version ${projectName}.jucer", 
							returnStdout: true
						).trim()						
					}
				}
            }
        }
        stage('Test') { 
            steps {
				script {
					echo "Validating plugin"
					if (!isUnix()) {
						bat """
							curl -L "https://github.com/Tracktion/pluginval/releases/download/latest_release/pluginval_Windows.zip" -o pluginval.zip
							jar xf pluginval.zip
							pluginval.exe --validate-in-process --validate "Installer/Windows/${projectName}.vst3"
						"""
					} else {
						sh """
							curl -L "https://github.com/Tracktion/pluginval/releases/download/latest_release/pluginval_macOS.zip" -o pluginval.zip
							unzip pluginval
							pluginval.app/Contents/MacOS/pluginval --validate-in-process --validate "Installer/Mac/${projectName}.vst3" || exit 1
						"""
					}
				}
            }
        }
		stage('Package') {
			steps {
				script {				
					if (!isUnix()) {
						echo "Packaging plugin for Windows"
						bat """
							curl -L https://jrsoftware.org/download.php/is.exe -o innosetup.exe
							innosetup.exe /portable=1 /silent /currentuser /dir="./innosetup"
						    "innosetup/iscc" /DVersion=${version} "Installer/Windows/${projectName}-installer.iss"
						"""
					} else {
						echo "Packaging plugin for Mac"						
						sh """
							cd Installer/Mac
							packagesbuild -v ${projectName}.pkgproj
						"""
					}
				}
			}
		}	
    }
}   
