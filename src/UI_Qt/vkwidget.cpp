#include "vkwidget.h"
#include "ui_vkwidget.h"

#include "lmms/Piano.h"
#include "lmms/midiconverter.h"

#include "../Misc/Master.h"
//#include <rtosc/ports.h>

VkWidget::VkWidget(QtOscWidget *parent) :
	QtOscWidget(parent),
	ui(new Ui::VkWidget)
{
	ui->setupUi(this);

	m_midiConverter = new MidiConverter();
	Piano* m_piano = new Piano(m_midiConverter);
	ui->widget->setPiano(m_piano);

	connect(m_midiConverter,
		SIGNAL(noteOn(unsigned char, unsigned char, int)),
		this, SLOT(noteOn(unsigned char, unsigned char, int)));
	connect(m_midiConverter,
		SIGNAL(noteOff(unsigned char)), this, SLOT(noteOff(unsigned char)));
}

VkWidget::~VkWidget()
{
	delete ui;
}

//#include <QDebug> // TODO

void VkWidget::noteOn(unsigned char note,
	unsigned char velocity,
	int masterkeyshift)
{
	// ZASF wants channel, note, velocity
	osc->sendMsgFromHere("/noteOn", "ccc", (char)0/*TODO*/, note, velocity);
}

void VkWidget::noteOff(unsigned char note)
{
	osc->sendMsgFromHere("/noteOff", "cc", char(0)/*TODO*/, note);
}
