import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	condition: Qt.core.qtConfig.contains("opengl")

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqwt" }
	Depends{ name: "iqwt3d" }

	Export{
		Depends{ name: "iqwt" }
		Depends{ name: "iqwt3d" }
	}
}
