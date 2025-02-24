#include "musto/state.h"

void State::setMustoApplication(MustoApplication* mustoApplication) 
{ 
	m_mustoApplication = mustoApplication; 
}

void PlayingState::update(const float dt)
{
	m_mustoApplication->m_mustoGame.update(dt);
}

void PlayingState::draw(sf::RenderTarget& target)
{
	m_mustoApplication->m_mustoGame.draw(target);
}

void PlayingState::processEvents(std::optional<sf::Event> event, sf::RenderWindow& window)
{
	m_mustoApplication->m_mustoGame.processEvents(event, window);

	if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
	{
		if (keyReleased->scancode == sf::Keyboard::Scancode::Escape) m_mustoApplication->transitionTo(new PauseState);
	}
}

PauseState::PauseState()
{
	m_background.setSize({ Config::windowSizef.x / 2, Config::windowSizef.y / 2 });
	m_background.setOrigin({ m_background.getSize().x / 2, m_background.getSize().y / 2 });
	m_background.setPosition({ Config::windowSizef.x / 2, Config::windowSizef.y / 2 });
	m_background.setOutlineThickness(Config::windowSizef.x / 300);
	m_background.setFillColor({ 47,79,79, 200 });
	m_background.setOutlineColor(sf::Color::White);

	for (int i{}; i < m_options.size(); i++)
	{
		sf::Text text(Config::font);
		text.setString(static_cast<std::string>(m_options[i]));
		text.setCharacterSize(Config::windowSize.y / 15);
		sf::FloatRect rc = text.getLocalBounds();
		text.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
		text.setPosition({ Config::windowSizef.x / 2,
						 ((Config::windowSizef.y / 2) / (m_options.size() + 1)) * (i + 1) + Config::windowSizef.y / 4 });
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(text.getCharacterSize() / 15);
		m_texts.push_back(text);
	}

	m_pauseText.setString("PAUSE");
	m_pauseText.setLetterSpacing(Config::windowSizef.x / 500);
	m_pauseText.setCharacterSize(Config::windowSizef.y / 12);
	sf::FloatRect rc = m_pauseText.getLocalBounds();
	m_pauseText.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
	m_pauseText.setPosition({ Config::windowSizef.x / 2, ((Config::windowSizef.y / 4) - m_pauseText.getCharacterSize()) });
	m_pauseText.setFillColor({240,214,106});
	m_pauseText.setOutlineColor({116,103,78});
	m_pauseText.setOutlineThickness(m_pauseText.getCharacterSize() / 15);
}

void PauseState::update(const float dt)
{

}

void PauseState::draw(sf::RenderTarget& target)
{
	m_mustoApplication->m_mustoGame.draw(target);

	target.draw(m_pauseText);
	target.draw(m_background);

	for (int i {}; i < m_texts.size(); i++)
	{
		if (m_selection == i) m_texts[i].setFillColor({ 93,182,180});
		else m_texts[i].setFillColor(sf::Color::White);
		target.draw(m_texts[i]);
	}

}
void PauseState::processEvents(std::optional<sf::Event> event, sf::RenderWindow& window)
{
	if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
	{
		if (keyReleased->scancode == sf::Keyboard::Scancode::I)
			std::cout << "INFORMATION : Your are in state PAUSE\n";
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Down) upSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Up) downSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Escape) m_mustoApplication->transitionTo(new PlayingState);
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Enter)
		{
			if (m_options[m_selection] == "Continue") m_mustoApplication->transitionTo(new PlayingState);
			else if (m_options[m_selection] == "Menu") m_mustoApplication->transitionTo(new MenuState);
		}
	}
}

void PauseState::upSelection() 
{
	if (m_selection >= m_options.size() - 1) m_selection = 0;
	else m_selection++;
}
void PauseState::downSelection() 
{
	if (m_selection <= 0) m_selection = m_options.size() - 1;
	else m_selection--;
}

MenuState::MenuState()
{	
	for (int i{}; i < m_options.size(); i++)
	{
		sf::Text text(Config::font);
		text.setString(static_cast<std::string>(m_options[i]));
		text.setCharacterSize(Config::windowSize.y / 10);
		sf::FloatRect rc = text.getLocalBounds();
		text.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
		text.setPosition({ Config::windowSizef.x / 2, (Config::windowSizef.y / (m_options.size() + 1)) * (i + 1) });
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(text.getCharacterSize() / 15);

		m_texts.push_back(text);
	}

	m_title.setString("MUSTOD");
	m_title.setLetterSpacing(Config::windowSizef.x / 200);
	m_title.setCharacterSize(Config::windowSizef.y / 4);
	sf::FloatRect rc = m_title.getLocalBounds();
	m_title.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
	m_title.setPosition({ Config::windowSizef.x / 2, (Config::windowSizef.y / (10 + 1))});
	m_title.setFillColor({240,214,106});
	m_title.setOutlineColor({116,103,78});
	m_title.setOutlineThickness(m_title.getCharacterSize() / 15);


}

void MenuState::update(const float dt)
{

}

void MenuState::draw(sf::RenderTarget& target)
{
	target.draw(m_title);

	for (int i {}; i < m_texts.size(); i++)
	{
		if (m_selection == i) m_texts[i].setFillColor({ 93,182,180});
		else m_texts[i].setFillColor(sf::Color::White);
		target.draw(m_texts[i]);
	}
}

void MenuState::processEvents(std::optional<sf::Event> event, sf::RenderWindow& window)
{
	if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
	{
		if (keyReleased->scancode == sf::Keyboard::Scancode::I)
			std::cout << "INFORMATION : Your are in state MENU, the current selection is " << m_options[m_selection] << "\n";
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Down) upSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Up) downSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Enter)
		{
			if (m_options[m_selection] == "Play") m_mustoApplication->transitionTo(new PlayingState);
			else if (m_options[m_selection] == "Quit") window.close();
		}
	}
}

void MenuState::upSelection() 
{
	if (m_selection >= m_options.size() - 1) m_selection = 0;
	else m_selection++;
}
void MenuState::downSelection() 
{
	if (m_selection <= 0) m_selection = m_options.size() - 1;
	else m_selection--;
}
