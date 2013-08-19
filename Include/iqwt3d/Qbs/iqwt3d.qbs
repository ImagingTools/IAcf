import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqwt3d"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtgui" }
	Depends{ name: "ibase" }
	Depends{ name: "imath" }
	Depends{ name: "qwt3d" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.opengl" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "ibase" }
		Depends{ name: "imath" }
		Depends{ name: "qwt3d" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.opengl" }
	}
}
