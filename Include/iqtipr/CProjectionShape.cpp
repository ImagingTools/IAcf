#include "iqtipr/CProjectionShape.h"


// Qt includes
#include <QStyleOptionGraphicsItem>
#include <QPainter>


namespace iqtipr
{


// public methods

CProjectionShape::CProjectionShape()
{
	setPen(QPen(QBrush(Qt::blue), 0));
	setZValue(1);
}


// reimplemented (imod::IObserver)

void CProjectionShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	iipr::CProjectionData* projectionPtr = GetObjectPtr();
	if (projectionPtr != NULL){
		iimg::IBitmap& bitmap = projectionPtr->GetProjectionImage();
		int lineSize = bitmap.GetLineBytesCount();
		QPainterPath path;
		if (!bitmap.IsEmpty()){
			const I_BYTE* imageLinePtr = (I_BYTE*)bitmap.GetLinePtr(0);
			for(int x = 1; x < lineSize - 1; x++){
				path.moveTo(x - 1, imageLinePtr[x - 1]);
				path.lineTo(x, imageLinePtr[x]);
			}

			setPath(path);
		}
		else{
			setPath(QPainterPath());
		}
	}
}


} // namespace iqtipr


