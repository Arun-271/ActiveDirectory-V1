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
    <form action="CreateUser" method="post">
    <div class="form">
        <div class="title">Create</div>
        <div class="subtitle">Create Active Directory User!</div>
        <div class="input-container ic1">
          <input id="firstname" class="input" type="text" placeholder=" " name="firstName"/>
          <div class="cut"></div>
          <label for="firstname" class="placeholder">First name</label>
        </div>
        <div class="input-container ic2">
          <input id="lastname" class="input" type="text" placeholder=" " name="lastName"/>
          <div class="cut"></div>
          <label for="lastname" class="placeholder">Last name</label>
        </div>
        <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="accountName"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">sAM Account Name</label>
          </div>
        <div class="input-container ic2">
            <input id="password" class="input" type="password" placeholder=" " name="intialPassword" />
            <div class="cut"></div>
            <label for="password" class="placeholder">Password</label>
          </div> 
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="desc"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Description</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="email" placeholder=" " name="mail"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">email</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="street"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Street</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="city"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">City</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="state"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">State</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="country"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Country</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="postalCode"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Postal code</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="telephone"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Telephone</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="mobile"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Mobile</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="title"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Job Title</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="department"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Department</label>
          </div>
          <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="company"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Company</label>
          </div>
              <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="manager"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Manager</label>
          </div>
        <input type="submit" class="submit" value="Submit">
      </div>
      </form>    
      <br>
</body>
</html>