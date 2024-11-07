#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

class jeu_couleur
{
public:

	jeu_couleur(int difficulte, Mat flux);
	double dist2color(Vec3b color_pixel, Vec3b spec_color);
    void closest2color(int &res, Vec3b color);
	void boucle();
	void afficherScore();
	void afficheBoutton();
	void add_score(int i) { d_score = d_score + i; }
	void dessinerBarreIntensite(int counter, int min, int max, Vec3b color, Point p1, Point p2);

	void dessineCercle(int x, int y);
	void decrementeY(int pas);
	
	void afficherCercle();
	void supprimerCercle(int index);
	bool verifCercle(int cercleIndex);

	void realMouse(int event, int x, int y, int flag) {
		if (event == cv::EVENT_LBUTTONDOWN) {

			if (x >= d_gauche_buttonNOcolor.x && x <= d_droite_buttonNOcolor.x &&
				y >= d_gauche_buttonNOcolor.y && y <= d_droite_buttonNOcolor.y)
			{
				std::cout << "Les coordonn�es se situent dans le rectangle rouge." << std::endl;
				if (d_vec_color.size() == 2)
				{
					closest2color(vert_nocolor, d_vec_color[1]);
					std::cout << "vert min" << vert_nocolor << std::endl;
				}
				else {
					closest2color(rouge_nocolor,d_vec_color[0]);
					std::cout << "rouge min" << rouge_nocolor << std::endl;
				}
				
			}
			else if (x >= d_gauche_buttonFULLcolor.x && x <= d_droite_buttonFULLcolor.x &&
				y >= d_gauche_buttonFULLcolor.y && y <= d_droite_buttonFULLcolor.y)
			{
				std::cout << "Les coordonn�es se situent dans le rectangle vert." << std::endl;
				if (d_vec_color.size() == 2)
				{
					closest2color(vert_fullcolor, d_vec_color[1]);
					std::cout << "vert max" << vert_fullcolor << std::endl;
				}
				else {
					closest2color(rouge_fullcolor, d_vec_color[0]);
					std::cout << "rouge max" << rouge_fullcolor << std::endl;
				}
			}
			else
			{

			
			cv::Vec3b clickedColor = d_flux.at<cv::Vec3b>(y, x);
			d_color = clickedColor;
			d_vec_color.push_back(clickedColor);

			std::cout << "Couleur du pixel cliqu� : (" << static_cast<int>(clickedColor[2]) << ", "
				<< static_cast<int>(clickedColor[1]) << ", "
				<< static_cast<int>(clickedColor[0]) << ")" << std::endl;
		}
		}
	}

	//stackoverflow

	// static wrapper (does NOT need "this")
	static void onMouse(int event, int x, int y, int flag, void* me) {
		// "me" is, what we fed into setMouseCallback() below:
		jeu_couleur* that = (jeu_couleur*)(me); // cast back to proper class
		that->realMouse(event, x, y, flag); // call class member
	}

private:

	int d_difficulte;
	int d_score;
	int d_pas;

	int rouge_nocolor = -1;
	int rouge_fullcolor = -2;

	int vert_nocolor = -1;
	int vert_fullcolor = -2;

	Mat d_flux;
	Vec3b d_color = (255,255,255);

	Point d_best;

	VideoCapture d_cap;

	int d_width;
	int d_height;

	int d_count;
	int d_red_count=0;
	int d_green_count=0;

	Point d_gauche_buttonNOcolor;
	Point d_droite_buttonNOcolor;
	Point d_gauche_buttonFULLcolor;
	Point d_droite_buttonFULLcolor;

	std::vector<Vec3b> d_vec_color;
	
	int d_x_circle;
	int d_y_circle= 70; 
	int d_radius_circle = 50;

	bool jouer = false;

	std::vector<Point*> d_cercle;
	std::vector<int> d_radius;

	int d_proba_generation = 40;
	double distance0;
	bool lose = false;
};



