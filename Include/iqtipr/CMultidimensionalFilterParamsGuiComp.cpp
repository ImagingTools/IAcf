#include "iqtipr/CMultidimensionalFilterParamsGuiComp.h"


// Qt includes
#include <QString>

// ACF includes
#include "istd/TChangeNotifier.h"

// ACF-Solutions includes
#include "imeas/IUnitInfo.h"

#include "iipr/IMultidimensionalFilterConstraints.h"


namespace iqtipr
{


CMultidimensionalFilterParamsGuiComp::CMultidimensionalFilterParamsGuiComp()
:	m_widthScaleFactor(1),
	m_heightScaleFactor(1)
{
}


// reimplemented (imod::IModelEditor)

void CMultidimensionalFilterParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));

	iipr::IMultidimensionalFilterParams* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	imath::CVarVector filterLengths = objectPtr->GetFilterLengths();

	if (filterLengths.GetElementsCount() >= 1){
		filterLengths[0] = FilterWidthSlider->value() / m_widthScaleFactor;
	}

	if (filterLengths.GetElementsCount() >= 2){
		filterLengths[1] = FilterHeightSlider->value() / m_heightScaleFactor;
	}

	if (filterLengths != objectPtr->GetFilterLengths()){
		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetFilterLengths(filterLengths);
	}
}


void CMultidimensionalFilterParamsGuiComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iipr::IMultidimensionalFilterParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imath::CVarVector& filterLengths = objectPtr->GetFilterLengths();

		int filterDimensionsCount = filterLengths.GetElementsCount();

		if (filterDimensionsCount >= 1){
			FilterWidthSlider->setValue(int(filterLengths[0] * m_widthScaleFactor + 0.5));
		}

		if (filterDimensionsCount >= 2){
			FilterHeightSlider->setValue(int(filterLengths[1] * m_heightScaleFactor + 0.5));
		}

		UpdateLabel();
	}
}


// protected methods

void CMultidimensionalFilterParamsGuiComp::UpdateLabel()
{
	FilterWidthValueLabel->setText(QString::number(FilterWidthSlider->value()));
	FilterHeightValueLabel->setText(QString::number(FilterHeightSlider->value()));
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CMultidimensionalFilterParamsGuiComp::OnGuiModelAttached()
{
	iipr::IMultidimensionalFilterParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker blockUpdate(this);

		const iipr::IMultidimensionalFilterConstraints* constraintsPtr = objectPtr->GetConstraints();

		int filterDimensionsCount = 0;
		if (constraintsPtr != NULL){
			filterDimensionsCount = constraintsPtr->GetFilterDimensionsCount();
		}
		else{
			const imath::CVarVector& filterLengths = objectPtr->GetFilterLengths();
			filterDimensionsCount = filterLengths.GetElementsCount();
		}

		if (filterDimensionsCount >= 1){
			QString unitName;
			if (constraintsPtr != NULL){
				FilterWidthLabel->setText(iqt::GetQString(constraintsPtr->GetFilterDescription(0)));

				const imeas::IUnitInfo& unitInfo = constraintsPtr->GetFilterUnitInfo(0);
				unitName = iqt::GetQString(unitInfo.GetUnitName());

				m_widthScaleFactor = unitInfo.GetDisplayMultiplicationFactor();
				istd::CRange valueRange = unitInfo.GetValueRange();

				FilterWidthSlider->setMinimum(int(valueRange.GetMinValue() * m_widthScaleFactor));
				FilterWidthSlider->setMaximum(int(valueRange.GetMaxValue() * m_widthScaleFactor));
			}
			else{
				FilterWidthSlider->setMinimum(1);
				FilterWidthSlider->setMaximum(100);

				m_widthScaleFactor = 1;
			}

			FilterWidthLabel->setVisible(true);
			FilterWidthSlider->setVisible(true);
			FilterWidthValueLabel->setVisible(true);
			WidthUnitNameLabel->setText(unitName);
			WidthUnitNameLabel->setVisible(!unitName.isEmpty());
		}
		else{
			FilterWidthLabel->setVisible(false);
			FilterWidthSlider->setVisible(false);
			FilterWidthValueLabel->setVisible(false);
			WidthUnitNameLabel->setVisible(false);
		}

		if (filterDimensionsCount >= 2){
			QString unitName;
			if (constraintsPtr != NULL){
				FilterHeightLabel->setText(iqt::GetQString(constraintsPtr->GetFilterDescription(1)));

				const imeas::IUnitInfo& unitInfo = constraintsPtr->GetFilterUnitInfo(1);
				unitName = iqt::GetQString(unitInfo.GetUnitName());

				m_heightScaleFactor = unitInfo.GetDisplayMultiplicationFactor();
				istd::CRange valueRange = unitInfo.GetValueRange();

				FilterHeightSlider->setMinimum(int(valueRange.GetMinValue() * m_heightScaleFactor));
				FilterHeightSlider->setMaximum(int(valueRange.GetMaxValue() * m_heightScaleFactor));
			}
			else{
				FilterHeightSlider->setMinimum(1);
				FilterHeightSlider->setMaximum(100);

				m_heightScaleFactor = 1;
			}

			FilterHeightLabel->setVisible(true);
			FilterHeightSlider->setVisible(true);
			FilterHeightValueLabel->setVisible(true);
			HeightUnitNameLabel->setText(unitName);
			HeightUnitNameLabel->setVisible(!unitName.isEmpty());
		}
		else{
			FilterHeightLabel->setVisible(false);
			FilterHeightSlider->setVisible(false);
			FilterHeightValueLabel->setVisible(false);
			HeightUnitNameLabel->setVisible(false);
		}
	}

	BaseClass::OnGuiModelAttached();
}


// protected slots

void CMultidimensionalFilterParamsGuiComp::on_FilterWidthSlider_valueChanged(int /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdate(this);

		UpdateModel();

		UpdateLabel();
	}
}


void CMultidimensionalFilterParamsGuiComp::on_FilterHeightSlider_valueChanged(int /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdate(this);

		UpdateModel();

		UpdateLabel();
	}
}


} // namespace iqtipr

