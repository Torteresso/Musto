#ifndef MUSTO_APPLICATION_H
#define MUSTO_APPLICATION_H

#include "musto/state.h"

class MustoApplication
{
public:
	MustoApplication(State* state) : m_state{ state }
	{
	}

	~MustoApplication()
	{
		delete m_state;
	}

	void update()
	{
		m_state->update();
	}

	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window)
	{
		m_state->processEvents(event, window);
	}

	void TransitionTo(State *state) 
	{
		if (m_state != nullptr) delete m_state;

		m_state = state;
		m_state->setTusmo(this);
	}
private:
	State* m_state;

};

#endif