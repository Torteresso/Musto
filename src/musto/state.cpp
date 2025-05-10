#include "musto/state.h"

PlayingState::PlayingState(MustoApplication* mustoApplication) : State(mustoApplication)
{

}

void PlayingState::update(const float dt)
{
	m_mustoApplication->m_mustoGame.update(dt);

	if (m_mustoApplication->m_mustoGame.getStatus() != MustoGame::Status::InProgress)
	{
		m_mustoApplication->transitionTo(new EndGameState(m_mustoApplication));
	}
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
		if (keyReleased->scancode == sf::Keyboard::Scancode::Escape) m_mustoApplication->transitionTo(new PauseState(m_mustoApplication));
	}
}

PauseState::PauseState(MustoApplication* mustoApplication) : State(mustoApplication)
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
		text.setString(Translation::translate(m_options[i]));
		text.setCharacterSize(Config::windowSize.y / 15);
		sf::FloatRect rc = text.getLocalBounds();
		text.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
		text.setPosition({ Config::windowSizef.x / 2,
						 ((Config::windowSizef.y / 2) / (m_options.size() + 1)) * (i + 1) + Config::windowSizef.y / 4 });
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(text.getCharacterSize() / 15);
		m_texts.push_back(text);
	}

	m_pauseText.setString(Translation::translate("Pause"));
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
		if (keyReleased->scancode == sf::Keyboard::Scancode::Down) upSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Up) downSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Escape) m_mustoApplication->transitionTo(new PlayingState(m_mustoApplication));
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Enter)
		{
			if (m_options[m_selection] == "Continuer") m_mustoApplication->transitionTo(new PlayingState(m_mustoApplication));
			else if (m_options[m_selection] == "Menu") m_mustoApplication->transitionTo(new MenuState(m_mustoApplication));
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

EndGameState::EndGameState(MustoApplication* mustoApplication) : State(mustoApplication)
{
	assert(m_mustoApplication->m_mustoGame.getStatus() != MustoGame::Status::InProgress);

	m_background.setSize({ Config::windowSizef.x / 2, Config::windowSizef.y / 2 });
	m_background.setOrigin({ m_background.getSize().x / 2, m_background.getSize().y / 2 });
	m_background.setPosition({ Config::windowSizef.x / 2, Config::windowSizef.y / 2 });
	m_background.setOutlineThickness(Config::windowSizef.x / 300);
	m_background.setFillColor({ 47,79,79, 200 });
	m_background.setOutlineColor(sf::Color::White);

	for (int i{}; i < m_options.size(); i++)
	{
		sf::Text text(Config::font);
		text.setString(Translation::translate(m_options[i]));
		text.setCharacterSize(Config::windowSize.y / 15);
		sf::FloatRect rc = text.getLocalBounds();
		text.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
		text.setPosition({ Config::windowSizef.x / 2,
						 ((Config::windowSizef.y / 2) / (m_options.size() + 1)) * (i + 1) + Config::windowSizef.y / 4 });
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(text.getCharacterSize() / 15);
		m_texts.push_back(text);
	}


	if (m_mustoApplication->m_mustoGame.getStatus() == MustoGame::Status::Lost)
			m_endGameText.setString(Translation::translate("Défaite, le mot était : ") + std::string(m_mustoApplication->m_mustoGame.getWord()));
	else 
			m_endGameText.setString(Translation::translate("Victoire, le mot était : ") + std::string(m_mustoApplication->m_mustoGame.getWord()));
	m_endGameText.setLetterSpacing(Config::windowSizef.x / 800);
	m_endGameText.setCharacterSize(Config::windowSizef.y / 12);
	sf::FloatRect rc = m_endGameText.getLocalBounds();
	m_endGameText.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
	m_endGameText.setPosition({ Config::windowSizef.x / 2, ((Config::windowSizef.y / 4) - m_endGameText.getCharacterSize()) });
	m_endGameText.setFillColor({240,214,106});
	m_endGameText.setOutlineColor({116,103,78});
	m_endGameText.setOutlineThickness(m_endGameText.getCharacterSize() / 15);
}

void EndGameState::update(const float dt)
{

}

void EndGameState::draw(sf::RenderTarget& target)
{
	m_mustoApplication->m_mustoGame.draw(target);

	target.draw(m_endGameText);
	target.draw(m_background);

	for (int i {}; i < m_texts.size(); i++)
	{
		if (m_selection == i) m_texts[i].setFillColor({ 93,182,180});
		else m_texts[i].setFillColor(sf::Color::White);
		target.draw(m_texts[i]);
	}

}
void EndGameState::processEvents(std::optional<sf::Event> event, sf::RenderWindow& window)
{
	if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
	{
		if (keyReleased->scancode == sf::Keyboard::Scancode::Down) upSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Up) downSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Enter)
		{
			if (m_options[m_selection] == "Rejouer")
			{
				m_mustoApplication->m_mustoGame.configureNewGame();
				m_mustoApplication->transitionTo(new PlayingState(m_mustoApplication));
			}
			else if (m_options[m_selection] == "Menu") m_mustoApplication->transitionTo(new MenuState(m_mustoApplication));
		}
	}
}

void EndGameState::upSelection() 
{
	if (m_selection >= m_options.size() - 1) m_selection = 0;
	else m_selection++;
}
void EndGameState::downSelection() 
{
	if (m_selection <= 0) m_selection = m_options.size() - 1;
	else m_selection--;
}

OptionState::OptionState(MustoApplication* mustoApplication) : State(mustoApplication)
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
		std::string str{Translation::translate(m_options[i])};
		switch (i)
		{
		case 0:
			str += std::to_string(Config::nbLetters);
			break;
		case 1:
			str += std::to_string(Config::nbTry);
			break;
		case 2:
			str += Translation::translate("Français");
			break;
		}
		text.setString(str);
		text.setCharacterSize(Config::windowSize.y / 15);
		sf::FloatRect rc = text.getLocalBounds();
		text.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
		text.setPosition({ Config::windowSizef.x / 2,
						 ((Config::windowSizef.y / 2) / (m_options.size() + 1)) * (i + 1) + Config::windowSizef.y / 4 });
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(text.getCharacterSize() / 15);
		m_texts.push_back(text);
	}


	m_optionText.setString(Translation::translate("Options"));
	m_optionText.setLetterSpacing(Config::windowSizef.x / 800);
	m_optionText.setCharacterSize(Config::windowSizef.y / 12);
	sf::FloatRect rc = m_optionText.getLocalBounds();
	m_optionText.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
	m_optionText.setPosition({ Config::windowSizef.x / 2, ((Config::windowSizef.y / 4) - m_optionText.getCharacterSize()) });
	m_optionText.setFillColor({240,214,106});
	m_optionText.setOutlineColor({116,103,78});
	m_optionText.setOutlineThickness(m_optionText.getCharacterSize() / 15);
}

void OptionState::update(const float dt)
{

}

void OptionState::draw(sf::RenderTarget& target)
{
	target.draw(m_optionText);
	target.draw(m_background);

	for (int i {}; i < m_texts.size(); i++)
	{
		if (m_selection == i) m_texts[i].setFillColor({ 93,182,180});
		else m_texts[i].setFillColor(sf::Color::White);
		target.draw(m_texts[i]);
	}

}
void OptionState::processEvents(std::optional<sf::Event> event, sf::RenderWindow& window)
{
	if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
	{
		if (keyReleased->scancode == sf::Keyboard::Scancode::Down) upSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Up) downSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Right) increaseSelection(m_selection);
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Left) decreaseSelection(m_selection);
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Enter)
		{
			if (m_options[m_selection] == "Retour") m_mustoApplication->transitionTo(new MenuState(m_mustoApplication));
		}
	}
}

void OptionState::upSelection() 
{
	if (m_selection >= m_options.size() - 1) m_selection = 0;
	else m_selection++;
}
void OptionState::downSelection() 
{
	if (m_selection <= 0) m_selection = m_options.size() - 1;
	else m_selection--;
}

void OptionState::increaseSelection(const int optionNumber)
{
	const int languageIndex{ Translation::languageToIndex.at(Config::language) };
	switch (optionNumber)
	{
	case 0:
		if (Config::nbLetters >= Config::extremumNbLetters.second) return;
		Config::nbLetters++;
		m_texts[optionNumber].setString(Translation::translate("Nombre de lettres : ") + std::to_string(Config::nbLetters));
		return;
	case 1:
		if (Config::nbTry >= Config::extremumNbTry.second) return;
		Config::nbTry++;
		m_texts[optionNumber].setString(Translation::translate("Nombre d'essais : ") + std::to_string(Config::nbTry));
		return;
	case 2:
		if (languageIndex >= Translation::supportedLanguages.size() - 1) Config::language = Translation::supportedLanguages[0];
		else Config::language = Translation::supportedLanguages[languageIndex + 1];
		m_texts[optionNumber].setString(Translation::translate("Langue : ") + Translation::translate("Français"));
		return;
	default:
		return;
	}
}

void OptionState::decreaseSelection(const int optionNumber)
{
	const int languageIndex{ Translation::languageToIndex.at(Config::language) };
	switch (optionNumber)
	{
	case 0:
		if (Config::nbLetters <= Config::extremumNbLetters.first) return;
		Config::nbLetters--;
		m_texts[optionNumber].setString(Translation::translate("Nombre de lettres : ") + std::to_string(Config::nbLetters));
		return;
	case 1:
		if (Config::nbTry <= Config::extremumNbTry.first) return;
		Config::nbTry--;
		m_texts[optionNumber].setString(Translation::translate("Nombre d'essais : ") + std::to_string(Config::nbTry));
		return;	
	case 2:
		if (languageIndex <= 0) Config::language = Translation::supportedLanguages.back();
		else Config::language = Translation::supportedLanguages[languageIndex - 1];
		m_texts[optionNumber].setString(Translation::translate("Langue : ") + Translation::translate("Français"));
		return;

	default:
		return;
	}
}

MenuState::MenuState(MustoApplication* mustoApplication) : State(mustoApplication)
{	
	m_title.setString("MUSTO");
	m_title.setLetterSpacing(Config::windowSizef.x / 200);
	m_title.setCharacterSize(Config::windowSizef.y / 4);
	sf::FloatRect rc = m_title.getLocalBounds();
	m_title.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
	m_title.setPosition({ Config::windowSizef.x / 2, (Config::windowSizef.y / (10 + 1))});
	m_title.setFillColor({240,214,106});
	m_title.setOutlineColor({116,103,78});
	m_title.setOutlineThickness(m_title.getCharacterSize() / 15);

	for (int i{}; i < m_options.size(); i++)
	{
		sf::Text text(Config::font);
		text.setString(Translation::translate(m_options[i]));
		text.setCharacterSize(Config::windowSize.y / 10);
		sf::FloatRect rc = text.getLocalBounds();
		text.setOrigin({ rc.size.x / 2, rc.size.y / 2 });
		text.setPosition({ Config::windowSizef.x / 2,
			((Config::windowSizef.y - m_title.getCharacterSize()) / (m_options.size() + 1)) * (i + 1) + m_title.getCharacterSize()});
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(text.getCharacterSize() / 15);

		m_texts.push_back(text);
	}

	m_background.setSize({ Config::windowSizef.x, Config::windowSizef.y });

	if (!m_backgroundShader.loadFromFile("res/mainShader.glsl", sf::Shader::Type::Fragment))
	{
		std::cout << "Error loading shader" << std::endl;
	}

	m_backgroundShader.setUniform("u_resolution", m_background.getSize());
	m_backgroundShader.setUniform("u_windowSize", Config::windowSizef);
	m_backgroundShader.setUniform("u_mouse", m_background.getPosition());

}

void MenuState::update(const float dt)
{
	m_backgroundShader.setUniform("u_time", m_mustoApplication->m_clock.getElapsedTime().asSeconds());
}

void MenuState::draw(sf::RenderTarget& target)
{
	target.draw(m_background, &m_backgroundShader);
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
		if (keyReleased->scancode == sf::Keyboard::Scancode::Down) upSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Up) downSelection();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::Enter)
		{
			if (m_options[m_selection] == "Jouer")
			{
				m_mustoApplication->m_mustoGame.configureNewGame();
				m_mustoApplication->transitionTo(new PlayingState(m_mustoApplication));
			}
			else if (m_options[m_selection] == "Mot du jour")
			{
				m_mustoApplication->m_mustoGame.configureNewGame(true);
				m_mustoApplication->transitionTo(new PlayingState(m_mustoApplication));
			}
			else if (m_options[m_selection] == "Options") m_mustoApplication->transitionTo(new OptionState(m_mustoApplication));
			else if (m_options[m_selection] == "Quitter") window.close();
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
