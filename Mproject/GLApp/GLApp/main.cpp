/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Main function.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <unistd.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include  "sdb_image.h"
#include "JsonParser.h"
#include "Movie.h"
#include "Utility.h"

using namespace std;

// Global variables
const int WIDTH = 1024;
const int HEIGHT = 800;
pair<int, int> CurrPosition = std::make_pair(0, 0); // sets to moviesGrid[0][0];

unsigned int texture[1];
unordered_map<int,vector<std::shared_ptr<Movie>>> moviesGrid;

typedef unordered_set<std::shared_ptr<Movie>>::iterator MovieSetIterator;
JsonParser::MovieContainer trendingMovieMap{};
JsonParser::MovieContainer personalizedCuratedMap{};
JsonParser::MovieContainer becauseYouSetMap{};
MovieSetIterator trendingMovieIter;
MovieSetIterator personalizeCuratedMoviesIter;
MovieSetIterator  becauseYouSetMoviesIter;

/** @brief sets up texture **/
void setUpTexture(std::shared_ptr<MovieImage> movieImage_ptr)
{
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			   movieImage_ptr->width(),
			   movieImage_ptr->height(), 0,
	           GL_RGB, GL_UNSIGNED_BYTE,
		       movieImage_ptr->image());
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

/** @brief download image from web then load texture **/
void loadTexture(const std::string & imgUrl)
{
	if (!Utility::downloadMovieImage(imgUrl)) {
		cout << "LOASTEXTURE::Utility::downloadMovieImage() Failed\n" << endl;
		return;
	}
   const string imgPath{ "movieImage.jpg" };
   auto movieImage_ptr = Utility::loadImageFromDirectory(imgPath);
   if (!movieImage_ptr) {
		cout << "LOASTEXTURE::Utility::loadImageFromDirectory() Failed\n" << endl;
		return;
   }
   setUpTexture(movieImage_ptr);
   movieImage_ptr.reset();
}

/** @brief creates 3 * 3 Grids then add movie image  plus 3 arrow triangles  **/
void createSquares(const float margin,
				   const float squareWidth,
				   const float topY,
				   const float bottomY,
				   const int   row,
			       unordered_set<std::shared_ptr<Movie>>::iterator iter )
{
		float x_start = 0.0;
		float x_end = squareWidth;
		while ( x_end < WIDTH  ) {
			moviesGrid[row].push_back(*iter);
			loadTexture(iter->get()->imgUrl()); iter++;
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0,0.0); glVertex3f(x_start+margin, bottomY, 0.0);  // left bottom
				glTexCoord2f(1.0,0.0); glVertex3f(x_end, bottomY, 0.0);  // right bottom
				glTexCoord2f(1.0,1.0); glVertex3f(x_end, topY, 0.0);  // right top
				glTexCoord2f(0.0,1.0); glVertex3f(x_start+margin, topY, 0.0);  // left top
			glEnd();
			x_start = x_end;
			x_end += squareWidth;
			
		}// end while loop.
}

/** @brief draw text on the screen **/
void renderBitmapString(float x, float y, void* font, const string& text)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2d(x, y);
	for (auto ch : text)
		glutBitmapCharacter(font, ch);
}


static bool draw{ true }; // draw function gets called once
/** @brief display function for GLUT  **/
void display(void)
{
	if (!draw)
		return;

	draw = false;
    glClearColor(0.0f, 0.05f, 0.13f,1.0f);         // disney plus background
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	renderBitmapString(20.0f, 700.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Trending");
	renderBitmapString(20.0f, 480.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Personalized Curated Movies");
	renderBitmapString(20.0f, 260.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Because You Set ...");


	glGenTextures(1,texture);
	const float Margin{ 10.0 };
	const float SquareWidth{ 250.0 };
	float stripBottomY{ 40 };
	float stripTopY{ 250 };
	bool first(true);

	// create 1st row
	createSquares(Margin, SquareWidth, stripTopY - Margin, stripBottomY + Margin,0, trendingMovieIter);
	stripBottomY = stripTopY + Margin;
	stripTopY += 220;

	// create 2nd row
	createSquares(Margin, SquareWidth, stripTopY - Margin, stripBottomY + Margin + 40,1, personalizeCuratedMoviesIter);
	stripBottomY = stripTopY + Margin;
	stripTopY += 220;

	// create 3rd row
	createSquares(Margin, SquareWidth, stripTopY - Margin, stripBottomY + Margin + 40,2, becauseYouSetMoviesIter );
	stripBottomY = stripTopY + Margin;
	stripTopY += 220;

	glutSwapBuffers(); // double buffered - swap the front and back buffers.
	glDeleteTextures(1, texture);
}

/** @brief resize callback  funciton **/
void onReshape(int newWidth, int newHeight)
{
	// ignore new width and height
	glutReshapeWindow(WIDTH, HEIGHT);

}

/*@brief Mouse input processing routin. for debug */
void onMouse(int buttonId, int buttonState, int x, int y) 
{
	if(buttonState != GLUT_DOWN)
		return;
	cout << "mouse coordinates -- (x,y): " << "(" << x << ", " << y << ")" << endl;
}

/* @ Keyboard input processing routine. work in progress **/
void keyInput(unsigned char key, int x, int y)
{
   std::cout << "x: " << x << "y: " << y << std::endl;
   switch(key) 
   {
      case 27: // esc key
         exit(0);
         break;
      default:
         break;
   }
}

/*
/** @brief Callback routine for non-ASCII key entry
 ** processing arrow keys
 **/
void specialKeyInput(int key, int x, int y)
{
   switch (key) {
	   case GLUT_KEY_UP:
		   CurrPosition.first++;
		   break;

	   case GLUT_KEY_DOWN:
		   CurrPosition.first--;
		   break;

	   case GLUT_KEY_LEFT:
		   CurrPosition.second--;
		   break;

	   case GLUT_KEY_RIGHT:
		   CurrPosition.second++;
		   break;

	   default:
		   break;
   }// end case switch

    // validate Columns
	if (CurrPosition.second >= (int)moviesGrid[0].size()  || CurrPosition.second < 0) {
		CurrPosition.second = 0;
	}
	// validate rows
	if (CurrPosition.first >= (int)moviesGrid.size()  || CurrPosition.first < 0) {
		CurrPosition.first = 0;
	}

	static bool firstCall{ true };
	if (firstCall) {
		firstCall = false;
		CurrPosition = { 0,0 };
	}
	cout << "You selected:\t";
	cout << moviesGrid[CurrPosition.first][CurrPosition.second].get()->title() << endl;
}


/** @brief main function **/
int main(int argc, char** argv)
{
	JsonParser json_parser;
	const string trendigRefId{ "25b87551-fd19-421a-be0f-b7f2eea978b3" };
	json_parser.readByRefId(trendigRefId);
	json_parser.getMovieSet(trendingMovieMap);
	trendingMovieIter = trendingMovieMap["TrendingSet"].begin();

	const string personalizedCuratedRefId{ "f506622c-4f75-4f87-bafe-3e08a4433914" }; json_parser.readByRefId(personalizedCuratedRefId); json_parser.getMovieSet(personalizedCuratedMap);
	personalizeCuratedMoviesIter = personalizedCuratedMap["PersonalizedCuratedSet"].begin();

	const string becauseYouSetRefId{ "bd1bfb9a-bbf7-43a0-ac5e-3e3889d7224d" };
	json_parser.readByRefId(becauseYouSetRefId);
	json_parser.getMovieSet(becauseYouSetMap);
	becauseYouSetMoviesIter = becauseYouSetMap["editorial"].begin();

	glutInit(&argc, argv);	// initializes GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		// enbale double buffered mode.
	glutInitWindowSize(WIDTH, HEIGHT)						;		// specifies window size
	glutInitWindowPosition(100, 100);						    	// position the window's initial top-left corner.
	glutCreateWindow("Disney Plus");								// create main window

	glMatrixMode(GL_MODELVIEW);										// setup viewing projection
	glLoadIdentity();												// start with identity matrix
	glOrtho(0.0, WIDTH, 0.0, HEIGHT, -1.0, 1.0);				    // set clipping area of 2D orthographic view
	//glutMouseFunc(onMouse);										    // registers mouse callback function.
	glutKeyboardFunc(keyInput);								        // registers keyboard event
	glutSpecialFunc(specialKeyInput);							    // registers function keys.
	glutReshapeFunc(onReshape);										// registers reshape callback function.
	glutDisplayFunc(display);								    	// registers display callback function.
	glutMainLoop();									         		// infinite main loop

	return EXIT_SUCCESS;
}

