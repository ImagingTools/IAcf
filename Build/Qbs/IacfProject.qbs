import qbs.base 1.0

Project{
	name: "IACF"

	Project{
		name: "IACF-Static"

		references: [
			"../../Include/iqwt/Qbs/iqwt.qbs",
			"../../Include/iqwt3d/Qbs/iqwt3d.qbs"
		]
	}

	Project{
		name: "IACF-Component"

		references: [
			"../../Impl/QwtPck/Qbs/QwtPck.qbs"
		]
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
		type: ["xpc", "acf_share"]

		files: ["../../Config/PartituraOnly.xpc"]

		destinationDirectory: "Config"

		Depends{ name: "QwtPck" }
		Depends{ name: "AcfSlnConfig" }
		Depends{ name: "acf" }
	}
}
