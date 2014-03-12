import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QwtPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqwt" }

	Export{
		Depends{ name: "iqwt" }
	}

	Group{
		condition: Qt.core.qtConfig.contains("opengl")

		Depends{ name: "iqwt3d" }

		Export{
			Depends{ name: "iqwt3d" }
		}
	}
}
