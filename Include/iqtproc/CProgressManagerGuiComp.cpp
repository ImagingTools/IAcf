#include "iqtproc/CProgressManagerGuiComp.h"


#include <QCoreApplication>


namespace iqtproc
{


CProgressManagerGuiComp::CProgressManagerGuiComp()
:	m_nextSessionId(0),
	m_progressSum(0),
	m_isCanceled(false)
{
}


// reimplemented (iproc::IProgressManager)

int CProgressManagerGuiComp::BeginProgressSession(const iser::CArchiveTag& /*progressTag*/)
{
	if (m_isCanceled){
		return -1;
	}

	int id = m_nextSessionId++;

	m_progressMap[id] = 0;

	if (IsGuiCreated()){
		ProgressBar->setEnabled(true);
		CancelButton->setEnabled(true);
		
		QCoreApplication::processEvents();
	}

	return id;
}


void CProgressManagerGuiComp::EndProgressSession(int sessionId)
{
	ProgressMap::iterator iter = m_progressMap.find(sessionId);
	I_ASSERT(iter != m_progressMap.end());

	m_progressSum -= iter->second;

	m_progressMap.erase(iter);

	if (m_progressMap.empty()){
		m_progressSum = 0;
		m_isCanceled = false;

		if (IsGuiCreated()){
			ProgressBar->setEnabled(false);
			ProgressBar->setValue(0);
			CancelButton->setEnabled(false);

			QCoreApplication::processEvents();
		}
	}
}


void CProgressManagerGuiComp::OnProgress(int sessionId, double currentProgress)
{
	ProgressMap::iterator iter = m_progressMap.find(sessionId);
	I_ASSERT(iter != m_progressMap.end());

	m_progressSum += currentProgress - iter->second;
	iter->second = currentProgress;

	if (IsGuiCreated()){
		UpdateProgressBar();

		QCoreApplication::processEvents();
	}
}


bool CProgressManagerGuiComp::IsCanceled(int /*sessionId*/) const
{
	return m_isCanceled;
}


// protected methods

void CProgressManagerGuiComp::UpdateProgressBar()
{
	if (!m_progressMap.empty()){
		ProgressBar->setValue(1000 * m_progressSum / m_progressMap.size());
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProgressManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	UpdateProgressBar();
}


// protected slots

void CProgressManagerGuiComp::on_CancelButton_clicked()
{
	CancelButton->setEnabled(false);

	m_isCanceled = true;
}


} // namespace iqtproc


