<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Active Directory</title>
    <link rel="styleSheet" href="main.css">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Poppins&display=swap" rel="stylesheet">
<style>
	#user-button-div button{
		background-color: #2c68d8;
		border-radius: 5px;
	}

	#user-button-div button:hover{
		background-color: #2053b1;
	}

	#users{
		width: 500px;
	}
</style>
</head>
    <h3 id="heading">Active Directory Management System</h3>
    <div>
    <div id="nav_container" >
        <ul id="nav">
            <a href="Index.jsp">
                <li><i class="glyphicon glyphicon-user" ><span style="font-family: sans-serif; margin-left: 10px;">   Create</span></i></li>
            </a>
            <a href="update.jsp">
                <li><i class="glyphicon glyphicon-edit" ><span style="font-family: sans-serif; margin-left: 10px;">   Update</span></i></li>
            </a>
            <a href="passwordReset.jsp">
                <li><i class="glyphicon glyphicon-remove-sign" ><span style="font-family: sans-serif; margin-left: 10px;">   Password Reset</span></i></li>
            </a>
            <a href="deleteUser.jsp">
                <li><i class="glyphicon glyphicon-trash" ><span style="font-family: sans-serif; margin-left: 10px;">   Delete</span></i></li>
            </a>   
            <a href=listUser.jsp>
                <li><i class="glyphicon glyphicon-user" ><span style="font-family: sans-serif; margin-left: 10px;">   List all user</span></i></li>
            </a> 
        </ul>
    </div>
    </div>
    <div  id="users">
    
    </div>
   <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.1/jquery.min.js"></script>
   <script>
   var div = document.getElementById('users');

   // getUserGeneralDetails related things

   function createEntry(label, value = "Not defined"){
		const entry = document.createElement("div");
		entry.classList.add("entry");

		entry.innerText = label + ":" + value;

		return entry
   }

   function getUserGeneralDetails(userName){
	   $.ajax({
		   url: '/ActiveDirectoryV2/GeneralDetails?user=' + userName,
		   type:'POST',
		   success: function(data){
			   data = JSON.parse(data);
			   console.log("done" + data)
			   const [firstName,intial ,lastName, displayName,desc, office,telephone, email,webpage] = data;

			   const entryDiv = document.querySelector('#entry-div')
			   entryDiv.innerHTML = ''
				entryDiv.append(createEntry("First name: ", firstName))
				entryDiv.append(createEntry("Intial: ", intial))
				entryDiv.append(createEntry("Last name: ", lastName))
				entryDiv.append(createEntry("Display name: ", displayName))
				entryDiv.append(createEntry("Description: ", desc))
				entryDiv.append(createEntry("Office: ", office))
				entryDiv.append(createEntry("Telephone Number: ", telephone))
				entryDiv.append(createEntry("Email: ", email))
				entryDiv.append(createEntry("Webpage: ", webpage))
		   }
	   })
   }

//    ----------------------------------------------------------------------------------------
   
// getAddressDetails related things
function getAddressDetails(userName){
	   $.ajax({
		   url: '/ActiveDirectoryV2/UserAddress?user=' + userName,
		   type:'POST',
		   success: function(data){
			   data = JSON.parse(data);
			   console.log("done" +data)
			   const [street, po,city, state, country, postalCode] = data;

			   const entryDiv = document.querySelector('#entry-div')
			   entryDiv.innerHTML = ''
				entryDiv.append(createEntry("street: ", street))
				entryDiv.append(createEntry("Postal box: ", po))
				entryDiv.append(createEntry("City: ", city))
				entryDiv.append(createEntry("State: ", state))
				entryDiv.append(createEntry("Country: ", country))
				entryDiv.append(createEntry("Postal Code: ", postalCode))
		   }
	   })
   }

   //---------------------------------------------------------------------------------
// Account related things
function getAccountDetails(userName){
	   $.ajax({
		   url: '/ActiveDirectoryV2/UserAccountDetails?user=' + userName,
		   type:'POST',
		   success: function(data){
			   data = JSON.parse(data);
			   console.log("done" +data)
			   const [samAccName, univeralAccName, passLastSet, accExpiryData] = data;

			   const entryDiv = document.querySelector('#entry-div')
			   entryDiv.innerHTML = ''
				entryDiv.append(createEntry("SAM Acc. Name: ", samAccName))
				entryDiv.append(createEntry("Universal Acc. Name: ", univeralAccName))
				entryDiv.append(createEntry("Password last set at: ", passLastSet))
				entryDiv.append(createEntry("Account expires at: ", accExpiryData))
		   }
	   })
   }

   //---------------------------------------------------------------------------------
// Phone related things
function getTelephoneDetails(userName){
	   $.ajax({
		   url: '/ActiveDirectoryV2/UserPhoneDetails?user=' + userName,
		   type:'POST',
		   success: function(data){
			   data = JSON.parse(data);
			   console.log("done" +data)
			   const [homePhone,pager, mobile,fax,ip] = data;

			   const entryDiv = document.querySelector('#entry-div')
			   entryDiv.innerHTML = ''
				entryDiv.append(createEntry("Home phone: ", homePhone))
				entryDiv.append(createEntry("Pager :", pager))
				entryDiv.append(createEntry("Mobile: ", mobile))
				entryDiv.append(createEntry("Fax: ", fax))
				entryDiv.append(createEntry("IP Phone: ", ip))
		   }
	   })
   }
// Org related things
function getOrgDetails(userName){
	   $.ajax({
		   url: '/ActiveDirectoryV2/UserOrganizationDetails?user=' + userName,
		   type:'POST',
		   success: function(data){
			   data = JSON.parse(data);
			   console.log("done" +data)
			   const [jobTitle, dept,company, manager] = data;

			   const entryDiv = document.querySelector('#entry-div')
			   entryDiv.innerHTML = ''
				entryDiv.append(createEntry("Job title: ", jobTitle))
				entryDiv.append(createEntry("Department: ", dept))
				entryDiv.append(createEntry("Company: ", company))
				entryDiv.append(createEntry("Manager Name: ", manager))
		   }
	   })
   }
// Get login Hrs
function getLoginHrs(userName){
	$.ajax({
		   url: '/ActiveDirectoryV2/LogonHours?user=' + userName,
		   type:'POST',
		   success: function(data){
			   data = JSON.parse(data);
			   console.log(data);
			   const entryDiv = document.querySelector('#entry-div')
			   entryDiv.innerHTML = '';
			   for(i = 0 ; i< data.length;i++){
				   let user = document.createElement("div");
				   user.innerHTML = data[i];
				   entryDiv.append(user);
			   }
		   }
	   })
}
function getDirectReports(userName){
	$.ajax({
		   url: '/ActiveDirectoryV2/DirectReports?user=' + userName,
		   type:'POST',
		   success: function(data){
			   data =  JSON.parse(data);
			   const entryDiv = document.querySelector('#entry-div')
			   entryDiv.innerHTML = '';
			   const head = document.createElement("div");
			   head.innerHTML = "Direct Reports to " + userName;
			   entryDiv.append(head);
			   for(i = 0 ; i< data.length;i++){
				   let user = document.createElement("div");
				   user.innerHTML = data[i];
				   entryDiv.append(user);
			   }
		   }
	   })
}
  function listusers(){
	   
	   $.ajax({
	       url:'/ActiveDirectoryV2/ListAllUsers',
	       type:'POST',
	       success: function(data){
	    	   data = JSON.parse(data);
	    	   div.innerText=" ";
	    	   for(i = 0;i <data.length;i++){
	    		   var user = document.createElement("div");
	    		   user.innerHTML = "<button onclick=detailView('"+data[i]+"') id="+data[i]+">"+data[i]+"</button>";
				   
	    		   div.appendChild(user);
	    		   
	    	   }
	    	   let img=document.getElementById("profile")
	    	   if(img!=null){
	    		   body=document.querySelector("body");
	    		   body.removeChild(img)
	    		   
	    	   }
	       }
	   });
	   
   }
   
   function detailView(id){
	  profilePicture(id);
	   $.ajax({
	       url:'/ActiveDirectoryV2/DetailedView?user='+id,
	       type:'POST',
	       success: function(data){
	    	data = JSON.parse(data);
	    	div.innerText=" "
			// changing here
			const buttons = [{label:"General", onclick:"getUserGeneralDetails"},{label: "Address",onclick:"getAddressDetails"},{label:"Account",onclick:"getAccountDetails"},{label:"Telephone",onclick:"getTelephoneDetails"},{label: "Organization",onclick:"getOrgDetails"},{label:"Login Hours",onclick:"getLoginHrs"},{label:"Direct Reports",onclick:"getDirectReports"}]
				const buttonDiv = document.createElement("div")
				buttonDiv.id = "user-button-div"
				let buttonDivInnerHtml = '';
				for(let button of buttons){
					const singleButtonHtml = "<button onclick="+button.onclick+"('"+id+"')>" + button.label + '</button>';
					buttonDivInnerHtml += singleButtonHtml
				}		
				buttonDiv.innerHTML = buttonDivInnerHtml;
				
				div.append(buttonDiv)
	    	// comment upto here

			const entryDiv = document.createElement("div");
			entryDiv.id = 'entry-div';
			div.appendChild(entryDiv)
	    	
	    	var b=document.createElement("button");
	 	   	b.innerHTML = "Back";
	 	  	b.setAttribute('onclick','listusers()');
	 	   	div.appendChild(b);
	 	   	
			// rendering the general page as default
			getUserGeneralDetails(id);
	     }
	   });
   }
   function profilePicture(id){
	   $.ajax({
	       url:'/ActiveDirectoryV2/ProfilePic?user='+id,
	       type:'POST',
	       success: function(data){
	    	   console.log("Profile sucess "+data)
	    	   data=data.replace("%20"," ")
	    	  body=document.querySelector("body");
	    	   var img=document.createElement("img");
	    	  img.setAttribute('src',"images/"+id+".jpg");
	    	   img.setAttribute('id',"profile");
	    	   img.setAttribute("style","width:300px;height:300px;position:absolute;bottom:30px;right:100px;")
	    	   body.appendChild(img);  
	    	  
	       }
	   });
   }
   
   listusers();
   </script>
</body>
</html>