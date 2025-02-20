#include "musto/mustoApplication.h"

MustoApplication::MustoApplication(State* state) : m_state {state}
{
	m_state->setMustoApplication(this);
}

MustoApplication::~MustoApplication()
{
	delete m_state;
}

void MustoApplication::update()
{
	m_state->update();
}

void MustoApplication::draw(sf::RenderTarget& target)
{
	m_state->draw(target);
}

void MustoApplication::processEvents(std::optional<sf::Event> event, sf::RenderWindow& window)
{
	m_state->processEvents(event, window);
}

void MustoApplication::transitionTo(State* state)
{
	if (m_state != nullptr) delete m_state;

	m_state = state;
	m_state->setMustoApplication(this);
}