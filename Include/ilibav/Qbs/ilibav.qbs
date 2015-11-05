import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
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
