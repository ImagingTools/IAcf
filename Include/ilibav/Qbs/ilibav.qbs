import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ilibav"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "imeas" }
	Depends{ name: "icam" }
	Depends{ name: "imm" }
	Depends{ name: "ilog" }
	Depends{ name: "ffmpeg" }

	Export{
		Depends{ name: "imeas" }
		Depends{ name: "icam" }
		Depends{ name: "imm" }
		Depends{ name: "ilog" }
		Depends{ name: "ffmpeg" }
	}
}
