import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "FfMpegPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilibav" }

	Export{
		Depends{ name: "ilibav" }
	}
}
