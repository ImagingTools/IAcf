import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QwtPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqwt" }

	Export{
		Depends{ name: "iqwt" }
	}
}
