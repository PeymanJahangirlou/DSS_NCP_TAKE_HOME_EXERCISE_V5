/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Simple Movie Class:
 */

#include "Movie.h"

/*@brief constructs a movie */
Movie::Movie(   const std::string& title,
			    const std::string& language,
				const std::string& sourceEntity,
				const std::string& imgUrl)
{
	M_title = title;
	M_language = language;
	M_sourceEntity = sourceEntity;
	M_imgUrl = imgUrl;
}
