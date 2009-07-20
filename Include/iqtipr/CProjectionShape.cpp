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
	resetTransform();

	iipr::CProjectionData* projectionPtr = GetObjectPtr();
	if (projectionPtr != NULL){
		iimg::IBitmap& bitmap = projectionPtr->GetProjectionImage();
		int lineSize = bitmap.GetLineBytesCount();
		QPainterPath path;

		QTransform transform;
		const i2d::CLine2d& projectionLine = projectionPtr->GetProjectionLine();

		double alpha = projectionLine.GetDiffVector().GetAngle();
	
		const i2d::CVector2d center = projectionLine.GetCenter();

		double scaleFactor = 1.0;
		
		if (lineSize > 0 && projectionLine.GetDiffVector().GetLength() > 0){
			scaleFactor = projectionLine.GetDiffVector().GetLength() / lineSize;
		}

		transform.translate(projectionLine.GetPoint1().GetX(), projectionLine.GetPoint1().GetY());
		transform.rotate(imath::GetDegreeFromRadian(alpha));
		transform.scale(scaleFactor, scaleFactor);
		transform.translate(0, -128);

		setTransform(transform);

		if (!bitmap.IsEmpty()){
			const I_BYTE* imageLinePtr = (I_BYTE*)bitmap.GetLinePtr(0);
			for(int x = 1; x < lineSize - 1; x++){
				path.moveTo(x - 1, 255 - imageLinePtr[x - 1]);
				path.lineTo(x, 255 - imageLinePtr[x]);
			}

			setPath(path);
		}
		else{
			setPath(QPainterPath());
		}
	}
}


} // namespace iqtipr


