#include <iostream>
#include <opencv2/opencv.hpp>
#include "jeu_couleur.h"

int main(int argc, char** argv)
{
    while (true)
    {
		std::cout << "Veuillez choisir votre niveau :" << std::endl;
		std::cout << "1) Facile - " << std::endl;
		std::cout << "2) Médium - " << std::endl;
		std::cout << "3) Difficile - " << std::endl;
		std::cout << "4) Impossible - Aucune limite" << std::endl;
		std::cout << "5) Quitter" << std::endl;

		int c;
		std::cin >> c;
		if (c == 5) {
			return 0;
		}
		else {
			Mat flux;
				jeu_couleur j{c,flux};
				j.boucle();
		}
    }

}
