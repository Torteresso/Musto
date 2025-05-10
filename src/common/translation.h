#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <array>
#include <map>
#include <string_view>
#include <cassert>

namespace Translation
{

	inline const std::map<std::string_view, int>		languageToIndex{ {"fr", 0}, {"en", 1} };
	constexpr std::array<std::string_view, 2>	supportedLanguages{ "fr", "en" };

	inline const std::map<std::string_view, std::array<std::string_view, supportedLanguages.size()>> translation

	{
		{"Jouer",						{"Jouer",		"Play"}},
		{"Options",						{"Options",		"Settings"}},
		{"Quitter",						{"Quitter",		"Quit"}},
		{"Retour",						{"Retour",		"Return"}},
		{"Continuer",					{"Continuer", "Continue"}},
		{"Menu",						{"Menu", "Main Menu"}},
		{"Pause",						{"Pause", "Pause"}},
		{"Rejouer",						{"Rejouer", "Play again" } },
		{"Nombre de lettres : ",		{"Nombre de lettres : ", "Number of letters : "}},
		{"Nombre d'essais : ",			{"Nombre d'essais : ", "Number of attempts : "}},
		{"Victoire, le mot était : ",	{"Gagné, le mot était : ", "You won, the word was : "}},
		{"Défaite, le mot était : ",	{"Perdu, le mot était : ", "You lost, the word was : "}},
		{"Langue : ", 					{"Langue : ", "Language : "}},
		{"Français",					{"Français", "English"}},
		{"Mot du jour",					{"Mot du jour", "Daily word"}}
	

	
	
					};

	inline const std::string translate(const std::string_view str)
	{
		//std::cout << str << "\n";
		assert(translation.contains(str) && "Il n'existe pas de traduction pour ce mot : ");

		return static_cast<std::string>(translation.at(str)[languageToIndex.at(Config::language)]);
	}

}

#endif