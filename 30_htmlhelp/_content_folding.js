/*********************************************************************************************
*  Copyright: 2008 by B&R Industrial Automation                                              *
*--------------------------------------------------------------------------------------------*
*  Identification:                                                                           *
*    System     Help System                                                                  *
*    Purpose	  folding and unfolding of html content                                      *
*                                                                                            *
*    Filename   content_floding.js                                                           *
*                                                                                            *
*--------------------------------------------------------------------------------------------*
*  History :                                                                                 *
*  Version       Datum         Autor                                                         *
*  01.00         19.08.08      W. Paulin         created                                     *
*  02.00         30.10.12      J. Gierlinger     add checkbox                                *
*********************************************************************************************/

/* expand and collapse topics */
function collapse_expand(conId, imgId)
{
  if( document.getElementById(conId).style.display != 'inline' )
	{
		document.getElementById(conId).style.display = 'inline';
		document.getElementById(imgId).src = '../icons/_collapse.png';
	}
  else
	{
		document.getElementById(conId).style.display = 'none';
		document.getElementById(imgId).src = '../icons/_expand.png';
	}

}

/* expand topics */
function expand(conId, imgId)
{
  if( document.getElementById(conId).style.display != 'inline' )
	{
		document.getElementById(conId).style.display = 'inline';
		document.getElementById(imgId).src = '../icons/_collapse.png';
	}
}

/* checkbox for optional parameters */
function checkbox(conId, imgId1)
{
  if( document.getElementById(conId).style.display != 'inline' )
	{
		document.getElementById(conId).style.display = 'inline';
		document.getElementById(imgId1).src = '../icons/_checked.png';
	}
  else
	{
		document.getElementById(conId).style.display = 'none';
		document.getElementById(imgId1).src = '../icons/_unchecked.png';
	}

}

/* Scroll view to topic */
function gotoId(conId)
{
    document.getElementById(conId).scrollIntoView('alignWithTop');
}

/* Navigation Table
$(document).ready(function(){
            $("#Navigation tr:odd").addClass("odd");
            $("#Navigation tr:not(.odd)").hide();
            $("#Navigation tr:first-child").show();
            
            $("#Navigation tr.odd").click(function(){
                // Save hidden status
                var washidden = ($(this).next("tr").is(":hidden"));
                
                // Close all tr and show header row
                $("#Navigation tr:not(.odd)").hide();
                $("#Navigation tr:first-child").show();
                // Reset all arrows
                $("#Navigation tr:odd").find(".arrow").attr("class","arrow");
                
                // If row was hidden, show and turn arrow
                if (washidden){
                    $(this).next("tr").show();
                    $(this).find(".arrow").addClass("up");
                }
            });
});
*/