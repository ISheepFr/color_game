#include "jeu_couleur.h"


jeu_couleur::jeu_couleur(int difficulte, Mat flux):d_difficulte{difficulte},d_flux{flux}
{
	d_color = (0, 0, 255);
	d_best.x = 0;
	d_best.y = 0;

	switch (difficulte) {
	case 1:
		d_pas = 1;
		d_proba_generation = 10;
		break;
	case 2:
		d_pas = 2;
	    d_proba_generation = 40;
		break;
	case 3: 
		d_pas = 4 ;
		d_proba_generation = 50;
		break;
	case 4:
		d_pas = 8;
		d_proba_generation = 60;
		break;
	default:
		d_pas = 1;
		d_proba_generation = 10;
		break;
	}

	if (d_cap.open(0))
	{
		// Obtenir la taille du cadre dans le flux vidéo
		d_width = d_cap.get(CAP_PROP_FRAME_WIDTH);
		d_height = d_cap.get(CAP_PROP_FRAME_HEIGHT);
		d_x_circle = d_width / 2;
	}

}

void jeu_couleur::dessineCercle(int x, int y)
{
	circle(d_flux, Point(x, y),d_radius_circle, Scalar(255, 0, 0),FILLED);
}

void jeu_couleur::supprimerCercle(int index)
{
	std::cout << index << std::endl;
	// Libérer la mémoire allouée pour le point
	delete d_cercle[index];

	// Supprimer le pointeur du vecteur
	d_cercle.erase(d_cercle.begin() + index);

	// Supprimer le rayon correspondant
	d_radius.erase(d_radius.begin() + index);
}

void jeu_couleur::decrementeY(int pas) 
{
	for (int i = 0; i < d_cercle.size(); i++)
	{
		d_cercle[i]->y = d_cercle[i]->y - pas;
		if (d_cercle[i]->y <= d_y_circle) {
			lose = true;
			break;
		}
	}
}

bool jeu_couleur::verifCercle(int cercleIndex)
{
	// Coordonnées du centre du cercle
	int centerX = d_cercle[cercleIndex]->x;
	int centerY = d_cercle[cercleIndex]->y;

	// Rayon du cercle
	int radius = d_radius[cercleIndex];

    double distance = std::sqrt(std::pow(centerX - d_x_circle, 2) + std::pow(centerY - d_y_circle, 2));
	//double distance0 = std::sqrt(std::pow(d_cercle[0]->x - d_x_circle, 2) + std::pow(d_cercle[0]->y - d_y_circle, 2));
	//std::cout << "distance zero : " << distance0 << " -- radius :" <std::cout << d_cercle[0]->x << "," << d_cercle[0]->y << std::endl;

	// Vérifier si le pixel est à l'intérieur du cercle
		if (distance <= (radius + d_radius_circle ))
		{
			return true;
		}
			
		
	
}

void jeu_couleur::afficherCercle()
{
	int r = rand() % 101;
	std::cout << "proba generer : " << r << std::endl;
	if (r <= d_proba_generation)
	{
		//if (d_nb_cercle_autorized != 0)
		//{
			//if (d_cercle.size() < d_nb_cercle_autorized)
			//{
				int radius = rand() % 20 + 1;
				Point* center = new Point;
				center->x = rand() % (d_width - 2 * radius) + radius;
				center->y = rand() % ( (d_height - (d_height / 4)) - 2 * radius) + (d_height - (d_height / 4)) + radius;

				d_cercle.push_back(center);
				std::cout << "PUSHBACKED CERCle" << std::endl;
				d_radius.push_back(radius);

				
				
			//}

		//}
		/*else {
			int radius = rand() % 50 + 1;
			Point* center = new Point;
			center->x = rand() % (d_width - 2 * radius) + radius;
			center->y = rand() % (d_height - 2 * radius) + radius;

			d_cercle.push_back(center);
			std::cout << "PUSHBACKED CERCle" << std::endl;
			d_radius.push_back(radius);

		}*/


	}

	for (int i = 0; i < d_cercle.size(); i++)
	{
		if (verifCercle(i))
		{
			add_score(1);
			supprimerCercle(i);
			break;
		}		
	}

	for (int i = 0; i < d_cercle.size(); i++)
	{
	  circle(d_flux, *d_cercle[i], d_radius[i], Scalar(0, 0, 255), FILLED);
	}

	


}

double jeu_couleur::dist2color(Vec3b color_pixel, Vec3b spec_color)
{
    int r_diff = color_pixel[2] - spec_color[2];
    int g_diff = color_pixel[1] - spec_color[1];
    int b_diff = color_pixel[0] - spec_color[0];

    return std::sqrt(static_cast<double>(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff));
}

void jeu_couleur::closest2color(int &res, Vec3b color)
{
	double count = 0;
    double minDistance = std::numeric_limits<double>::max();
    for (int y = 0; y < d_flux.rows; ++y) {
        for (int x = 0; x < d_flux.cols; ++x) {

            cv::Vec3b currentColor = d_flux.at<cv::Vec3b>(y, x);
            double distance = dist2color(currentColor, color);

            if (distance < minDistance) {
                minDistance = distance;
                d_best.x = x;
                d_best.y = y;
            }
			
			if (distance < 20)
			{
				count++;
			}
        }
    }
	
	res = count;
	std::cout << res << std::endl;
	
}

void jeu_couleur::afficherScore()
{
	Point textPosition(d_width - 160, 50);
	std::string scoreStr = "Count: " + std::to_string(d_score);
	Size textSize = getTextSize(scoreStr, FONT_HERSHEY_SIMPLEX, 1, 2, nullptr);

	Point rectTopLeft(textPosition.x - 10, textPosition.y - textSize.height - 10);
	Point rectBottomRight(textPosition.x + textSize.width + 10, textPosition.y + 10);

	rectangle(d_flux, rectTopLeft, rectBottomRight, Scalar(0, 255, 0), -1); // -1 remplit le rectangle

	putText(d_flux, scoreStr, textPosition, FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
}

void jeu_couleur::afficheBoutton()
{
	// Suppose que tu as déjà une image (par exemple, nommée "image") sur laquelle tu veux dessiner les rectangles.

	// Dimensions des rectangles
	int largeurRectangle = 50;
	int hauteurRectangle = 30;
	int espacement = 10;

	// Coordonnées du coin bas-droit de l'image
	int coinDroitX = d_flux.cols - 10;
	int coinBasY = d_flux.rows - 10;

	// Rectangle rouge
	cv::rectangle(d_flux, cv::Point(coinDroitX - (2 * largeurRectangle + espacement), coinBasY - hauteurRectangle),
		cv::Point(coinDroitX - (largeurRectangle + espacement), coinBasY), cv::Scalar(0, 0, 255), -1); // Rouge en BGR

	// Rectangle vert
	cv::rectangle(d_flux, cv::Point(coinDroitX - largeurRectangle, coinBasY - hauteurRectangle),
		cv::Point(coinDroitX, coinBasY), cv::Scalar(0, 255, 0), -1); // Vert en BGR

	// Rectangle rouge
	d_gauche_buttonNOcolor = cv::Point(coinDroitX - (2 * largeurRectangle + espacement), coinBasY - hauteurRectangle);
	d_droite_buttonNOcolor = cv::Point(coinDroitX - (largeurRectangle + espacement), coinBasY);

	// Rectangle vert
	d_gauche_buttonFULLcolor = cv::Point(coinDroitX - largeurRectangle, coinBasY - hauteurRectangle);
	d_droite_buttonFULLcolor = cv::Point(coinDroitX, coinBasY);
}

void jeu_couleur::dessinerBarreIntensite(int counter,int min, int max, Vec3b color, Point x, Point x2)
{
		int largeurImage = d_flux.cols;
		int hauteurImage = d_flux.rows;

		int hauteurBarre = static_cast<int>(static_cast<float>(d_red_count - rouge_nocolor) / (rouge_fullcolor - rouge_nocolor) * hauteurImage);
		cv::rectangle(d_flux, cv::Point(0, hauteurImage), cv::Point(10, hauteurImage - hauteurBarre),d_vec_color[0], -1); // Rouge // Rouge

		std::cout << "MIN ROUGE : " << rouge_nocolor << " | CURRENT ROUGE: " << d_red_count << " | MAX ROUGE: " << rouge_fullcolor << std::endl;

		hauteurBarre = static_cast<int>(static_cast<float>(d_green_count - vert_nocolor) / (vert_fullcolor - vert_nocolor) * hauteurImage);
		cv::rectangle(d_flux, cv::Point(largeurImage -10, hauteurImage), cv::Point(largeurImage, hauteurImage - hauteurBarre), d_vec_color[1], -1);
		
		std::cout << "MIN VERT : " << vert_nocolor << " | CURRENT VERT: " << d_green_count << " | MAX VERT: " << vert_fullcolor << std::endl;

		std::cout << "----------------" << std::endl;


		//cv::rectangle(d_flux, Point(x,hauteurImage - 20), Point(x2, hauteurImage), color, -1);
		
}




void jeu_couleur::boucle()
{
	if (d_cap.open(0))
	{
		namedWindow("Flux");
		setMouseCallback("Flux", jeu_couleur::onMouse, this);
	}

	while (true)
	{
		d_cap >> d_flux;

		flip(d_flux, d_flux, 1);

		afficheBoutton();

		//cv::flip(d_flux, d_flux, 1);
		
		
		if (d_vec_color.size() == 2 && (rouge_nocolor != -1 && vert_nocolor != -1 && rouge_fullcolor != -2 && vert_fullcolor != -2))
		{
			closest2color(d_red_count,d_vec_color[0]);
			closest2color(d_green_count, d_vec_color[1]);
			afficherScore();
			dessinerBarreIntensite(d_red_count,rouge_nocolor,rouge_fullcolor,d_vec_color[0], Point(0, d_height), Point(10, d_width) );
			
		}

		if (waitKey(30) == 112 || jouer)
		{
			jouer = true;
			int proportionRed = 5;    // 10% de la différence contribue à la mise à jour de x
			int proportionGreen = 5;
			int x = d_x_circle;
			x -= (proportionRed * (d_red_count - d_green_count)) / 100;  // Soustrait proportionnellement à la différence entre red et green
			x += (proportionGreen * (d_green_count - d_red_count)) / 100;  // Ajoute proportionnellement à la différence entre green et red
			afficherScore();
			d_x_circle = x;
			if (d_x_circle < 0)
			{
				d_x_circle = 0;
			}
			else if (d_x_circle > d_width) {
				d_x_circle = d_width;
			}
			
			dessineCercle(d_x_circle, d_y_circle);
			
			afficherCercle();
			
			decrementeY(d_pas);
			
			if (lose == true) {
				std::cout << "PERDU ! Votre score : " << d_score << std::endl;

				break;
			}
		}
		
		//circle(d_flux,d_best,10,Scalar(0,0,255));
	
		//std::cout << "MIN COLOR : " << rouge_fullcolor << " MAX COLOR : " << rouge_fullcolor << " CURRENT : ---- " << d_count << std::endl;
		

		imshow("Flux", d_flux);

		if (waitKey(30) == 27)
			break;
			
		
	}

	d_cap.release();
	destroyAllWindows();
}