import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "FfMpegPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilibav" }

	Export{
		Depends{ name: "ilibav" }
	}
}
