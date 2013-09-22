import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iqwt"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "imeas" }
	Depends{ name: "iqtgui" }
	Depends{ name: "ibase" }
	Depends{ name: "qwt" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "imeas" }
		Depends{ name: "iqtgui" }
		Depends{ name: "ibase" }
		Depends{ name: "qwt" }
		Depends{ name: "Qt.gui" }
	}
}
