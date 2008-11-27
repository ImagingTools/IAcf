#define AVTDIR GetEnv("AVTDIR")

[Setup]
AppVerName=IACF$WCMIXED? <Dirty>:$$WCMODS? <Local>:$
AppVersion=0.6
AppPublisher=ImagingTools
AppPublisherURL=http://www.imagingtools.de/
AppName=IACF

DefaultDirName={sd}\ImagingTools\IACF
WizardImageFile=..\..\Docs\Images\InstallerImage.bmp
WizardSmallImageFile=..\..\Docs\Images\Logo\Logo_128x128.bmp
SetupIconFile=..\..\Docs\Images\Logo\Logo.ico
AlwaysShowDirOnReadyPage=True
DefaultGroupName=ImagingTools\IACF
DisableProgramGroupPage=True
UninstallDisplayIcon=..\..\Docs\Images\Logo\Logo.ico
UninstallDisplayName=IACF
OutputDir=..\Setup
LicenseFile=..\..\IACF-License.txt
InfoAfterFile=..\InfoAfter.txt

[Files]
Source: ..\Temp\*; DestDir: {app}; Flags: recursesubdirs; Components: iacfComp

Source: {#AVTDIR}\FireGrab\*.dll; DestDir: {app}\ExtLib\Bin; Components: avtComp
Source: {#AVTDIR}\*; DestDir: {app}\ExtLib\AVT; Flags: recursesubdirs; Excludes: *.dll; Components: avtComp

[Languages]
Name: en; MessagesFile: compiler:Default.isl
Name: de; MessagesFile: compiler:Languages\German.isl
Name: pl; MessagesFile: compiler:Languages\Polish.isl

[CustomMessages]
en.MinimalType=Minimal installation
de.MinimalType=Minimale Installation
pl.MinimalType=Instalacja minimalna

en.FullType=Full installation
de.FullType=Komplete Installation
pl.FullType=Instalacja kompletna

en.CustomType=Custom installation
de.CustomType=Ausgewählte Komponenten
pl.CustomType=Wybrane komponenty

en.AvtComponents=AVT Camera components
de.AvtComponents=AVT Kamerakomponenten
pl.AvtComponents=Komponenty dla kamer firmy AVT

[Types]
Name: minimalType; Description: {cm:MinimalType}
Name: fullType; Description: {cm:FullType}
Name: customType; Description: {cm:CustomType}; Flags: iscustom

[Components]
Name: iacfComp; Description: IACF; Types: minimalType fullType customType
Name: avtComp; Description: {cm:AvtComponents}; Types: fullType customType

[Registry]
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: IACFDIR; ValueData: {app}; ValueType: string; Flags: uninsdeletevalue; Components: iacfComp
Root: HKLM; Subkey: SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\; ValueName: AVTDIR; ValueData: {app}\ExtLib\AVT; ValueType: string; Flags: uninsdeletevalue; Components: avtComp
