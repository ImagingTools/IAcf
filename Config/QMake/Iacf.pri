# Base configuration should be included for all projects used IACF

INCLUDEPATH += $$PWD/../../Include
INCLUDEPATH += $$PWD/../../Impl
INCLUDEPATH += $$PWD/../../$$AUXINCLUDEDIR

# get build output directory of shadow build and add include path
ACFDIRBUILD = $$(ACFDIR_BUILD)
!isEmpty(ACFDIRBUILD){
	INCLUDEPATH += $$(ACFDIR_BUILD)/../Iacf/$$AUXINCLUDEDIR
	LIBS += -L$$(IACFDIR_BUILD)/Lib/$$COMPILER_DIR
}
else{
	LIBS += -L$$PWD/../../Lib/$$COMPILER_DIR
}

