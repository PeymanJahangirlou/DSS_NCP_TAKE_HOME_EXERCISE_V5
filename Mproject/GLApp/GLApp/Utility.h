/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Utility class -- contains utility functions
 */

#ifndef Utility_H
#define Utility_H

#define CURL_STATICLIB
#include <curl\curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"advapi32.lib")

#include "sdb_image.h"
#include "MovieImage.hpp"

using std::string;
using std::ostream;


class Utility
{
public:

	/** @brief helper function -> saves loaded image from we to solution direction **/
	static size_t M_curlSaveImgToFile(void* buffer, size_t size, size_t nmemb, void* userp);

	/** @brief sets up curl to download image from provided imgUrl **/
	static CURLcode downloadMovieImage(const string& imgUrl, ostream& os, long timeout = 30);

	/** @brief download movie image from provided imgUrl to solution directory **/
	static bool  downloadMovieImage(const string& imgUrl);

	/** @brief load image from directory
     * @return MovieImage pointer
     */
	static std::shared_ptr<MovieImage> loadImageFromDirectory(const string& imgPath);




};

#endif // !Utility_H
