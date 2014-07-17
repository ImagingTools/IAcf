import qbs.base 1.0

Project{
	name: "IACF"

	property string projectRoot: path + "/../../.."

	Project{
		name: "IACF-Static"

		Project{
			name: "VC"
			condition: qbs.toolchain.contains("msvc")
			references: [
				"../../Include/iqwt/Qbs/iqwt.qbs",
				"../../Include/iqwt3d/Qbs/iqwt3d.qbs",
				"../../Include/ilibav/Qbs/ilibav.qbs"
			]
		}
	}

	Project{
		name: "IACF-Component"

		Project{
			name: "VC"
			condition: qbs.toolchain.contains("msvc")
			references: [
				"../../Impl/QwtPck/Qbs/QwtPck.qbs",
				"../../Impl/FfMpegPck/Qbs/FfMpegPck.qbs"
			]
		}
	}
/*
	Project{
		name: "IACF-Tool"

		references: [
//			"../../Impl/AcfStarterExe/Qbs/AcfStarter.qbs"
		]
	}
*/

	Product{
		name: "IacfConfig"
		condition: qbs.toolchain.contains("msvc")
		type: ["xpc", "acf_share"]

		files: ["../../Config/PartituraOnly.xpc"]

		destinationDirectory: "Config"

		Depends{
			name: "QwtPck"
			required: false
		}
		Depends{ name: "FfMpegPck" }
		Depends{ name: "AcfSlnConfig" }
		Depends{ name: "acf" }
	}
}
