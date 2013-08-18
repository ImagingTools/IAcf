import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqwt"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqtgui" }
	Depends{ name: "ibase" }
	Depends{ name: "qwt" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "ibase" }
		Depends{ name: "qwt" }
	}
}
