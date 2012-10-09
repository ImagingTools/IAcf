#define AVTDIR GetEnv("AVTDIR")

[Setup]
AppVerName=$AcfProductName$ $AcfVersion:10$
AppVersion=$AcfVersion:10$
AppPublisher=Witold Gantzke & Kirill Lepskiy
AppPublisherURL=http://www.imagingtools.de/
AppName=IACF
OutputBaseFilename=Setup_$AcfProductName$_$LICENSE_TYPE$_v$AcfVersion:10$_$CompilerName$

DefaultDirName={pf}\ImagingTools\$AcfProductName$_v$AcfVersion:0$
WizardImageFile=..\WizardImageFile.bmp
WizardSmallImageFile=..\WizardSmallImageFile.bmp
SetupIconFile=..\..\Docs\Images\Icons\Logo.ico
AlwaysShowDirOnReadyPage=True
DefaultGroupName=ImagingTools\IACF
DisableProgramGroupPage=True
UninstallDisplayIcon=..\..\Docs\Images\Icons\Logo.ico
UninstallDisplayName=$AcfProductName$ $AcfVersion:10$
OutputDir=..\Setup
LicenseFile=$LicensePath$
VersionInfoDescription=$AcfProductName$ v. $AcfVersion:10$ ($AcfRawVersion:10$) by $AcfCompanyName$
AppComments=compiled using $CompilerName$, based on ACF v. $AcfVersion:0$ ($AcfRawVersion:0$) and ACF-Solutions v. $AcfVersion:1$ ($AcfRawVersion:1$), see www.imagingtools.de to find out more about IACF
AppCopyright=Copyright (C) 2007-2011 Witold Gantzke and Kirill Lepskiy
VersionInfoVersion=$AcfVersion:10$
ChangesEnvironment=yes

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

[Icons]
Name: "{group}\API Documentation"; Filename: {app}\Docs\TechnicalDoc\index.html; Components: iacfComp

[Registry]
Root: HKCU; Subkey: "Environment"; ValueName: IACFDIR; ValueData: {app}; ValueType: string; Flags: uninsdeletevalue; Components: iacfComp
Root: HKCU; Subkey: "Environment"; ValueName: AVTDIR; ValueData: {app}\ExtLib\AVT; ValueType: string; Flags: uninsdeletevalue; Components: avtComp
