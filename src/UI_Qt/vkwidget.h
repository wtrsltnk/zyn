#ifndef VKWIDGET_H
#define VKWIDGET_H

#include "qtoscnode.h"

class Piano;
class MidiConverter;

namespace Ui {
	class VkWidget;
}

/**
 * @brief The VkWidget class
 *
 * This class also handles all connection to LMMS' PianoView
 */
class VkWidget : public QtOscWidget
{
	Q_OBJECT
	MidiConverter* m_midiConverter;

public:
	explicit VkWidget(QtOscWidget* parent = 0);
	~VkWidget();
	
private:
	Ui::VkWidget *ui;
private slots:
	void noteOn(unsigned char note,
		unsigned char velocity,
		int masterkeyshift);
	void noteOff(unsigned char note);
};

#endif // VKWIDGET_H
