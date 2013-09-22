import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "QwtPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqwt" }
	Depends{ name: "iqwt3d" }

	Export{
		Depends{ name: "iqwt" }
		Depends{ name: "iqwt3d" }
	}
}
