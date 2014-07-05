import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QwtPck"
	condition: Qt.core.qtConfig.contains("opengl")

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqwt" }
	Depends{
		name: "iqwt3d"
		required: false
	}

	Export{
		Depends{ name: "iqwt" }
		Depends{
			name: "iqwt3d"
			required: false
		}
	}
}
