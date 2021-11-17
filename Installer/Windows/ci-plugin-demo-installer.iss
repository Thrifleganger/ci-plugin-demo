#define Name "ci-plugin-demo"
#define Publisher "ADC"
#define PublisherUrl ""
#define SupportUrl ""
#define PublisherContact ""
#define AppDirectory="{commonpf64}\" + Publisher + "\" + Name

#ifndef Version
#define Version "1.0.0"
#endif

[Setup]
AppId={#Name}
AppName={#Name}
AppVersion={#Version}
AppPublisher={#Publisher}
AppPublisherURL={#PublisherUrl}
AppSupportURL={#SupportUrl}
AppContact={#PublisherContact}

CreateAppDir=no
OutputBaseFilename="{#Name}-{#Version}"
Compression=lzma
SolidCompression=yes
WizardStyle=modern

Uninstallable=yes
CreateUninstallRegKey=yes
UninstallDisplayName="{#Name}"
UninstallFilesDir="{#AppDirectory}"
DisableWelcomePage=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files] 
Source: "{#name}.vst3"; DestDir: "{commoncf64}\VST3\{#Publisher}"