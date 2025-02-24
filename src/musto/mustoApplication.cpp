#include "musto/mustoApplication.h"

MustoApplication::MustoApplication() : m_state {new MenuState(this)}
{
}

MustoApplication::~MustoApplication()
{
	delete m_state;
}

void MustoApplication::update(const float dt)
{
	m_state->update(dt);
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
}