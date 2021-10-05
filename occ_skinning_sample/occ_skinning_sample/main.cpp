#include "Viewer.h"

#include <BRepTools.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <IGESControl_Reader.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <Standard_Handle.hxx>
#include <TopoDS_Builder.hxx>


int main(int argc, char** argv)
{
	Viewer vout(50, 50, 500, 500);
	TopoDS_Shape fromFile;

	// Read iges
	IGESControl_Reader aReader_Iges;
    auto if_select_return_status = aReader_Iges.ReadFile(
		"C:\\Users\\huat-wuxi001\\Documents\\OneDrive\\HUSTWUXI\\repos\\"
		"code_repos\\CppProjects\\occt_skinning\\blade.igs");

	// Check entities
	IFSelect_PrintCount mode = IFSelect_ItemsByEntity;
	aReader_Iges.PrintCheckLoad(Standard_False, mode);

	// Count number of roots to transfer
	Standard_Integer numberOfRoots = aReader_Iges.TransferRoots();
	std::cout << "Number of roots = " << numberOfRoots << std::endl << std::endl;

	TopoDS_Shape aShape_Iges = aReader_Iges.OneShape();


	BRep_Builder B;
	TopoDS_Edge E;

	// explore each entity from TopoDS_shape
	TopExp_Explorer myFaceExplorer(aShape_Iges, TopAbs_FACE);
	while (myFaceExplorer.More())
	{
		double u0, u1;
		auto face = TopoDS::Face(myFaceExplorer.Current());
		auto surface = BRep_Tool::Surface(face);


		//check the close parameter line
		Standard_Boolean bUClose = surface->IsUClosed();
		Standard_Boolean bVClose = surface->IsVClosed();

		//check the periodic
		Standard_Boolean bUPeriodic = surface->IsUPeriodic();
		Standard_Boolean bVPeriodic = surface->IsVPeriodic();

		// Evaluate iso-V
		Handle(Geom_Curve) isoUCurve = surface->UIso(0.3);

		B.MakeEdge(E, isoUCurve, 0.001);
		// Evaluate a point on surface
		gp_Pnt pt;
		surface->D0(0.0, 0.0, pt);
		myFaceExplorer.Next();
	}
	vout << E;
    vout << aShape_Iges;
	
  
	vout.StartMessageLoop();

	return 0;
} 
