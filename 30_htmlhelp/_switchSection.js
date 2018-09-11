/*********************************************************************************************
*  Copyright: 2012 by B&R Industrial Automation                                              *
*--------------------------------------------------------------------------------------------*
*  Identification:                                                                           *
*    System     Help System                                                                  *
*    Purpose	switch between two topics				                                     *
*                                                                                            *
*    Filename   _switchSection.js                                                            *
*                                                                                            *
*--------------------------------------------------------------------------------------------*
*  History :                                                                                 *
*  Version       Datum         Autor                                                         *
*  01.00         01.08.12     L. Grieﬂler     created                                        *
*********************************************************************************************/

/* switch between two topics */
function switchSection(conId1, conId2, imgId)
{
  	if( document.getElementById(conId1).style.display != "inline" )
	{
		document.getElementById(conId1).style.display = "inline";
		document.getElementById(conId2).style.display = "none";
		document.getElementById(imgId).src = '../icons/_checked.png';
	}
	else
	{
		document.getElementById(conId2).style.display = "inline";
		document.getElementById(conId1).style.display = "none";
		document.getElementById(imgId).src = '../icons/_unchecked.png';
	}
}