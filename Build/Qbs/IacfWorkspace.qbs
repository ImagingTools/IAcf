import qbs.base 1.0

Project{
	property string acfDir: qbs.getenv("ACFDIR")
	property string acfSlnDir: qbs.getenv("ACFDIR")
	property string extLibDir: qbs.getenv("EXTLIBDIR")

	qbsSearchPaths: [
		acfDir + "/Config/Qbs",
		acfDir + "/Config/Qbs/share",
		acfSlnDir + "/Config/Qbs/share",
		extLibDir + "/Config/Qbs/share"
	]

	SubProject{
		filePath: "./IacfProject.qbs"
	}
}
