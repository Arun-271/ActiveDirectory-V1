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
    <form action="Delete" method="post">
    <div class="form">
        <div class="title">Delete</div>
        <div class="subtitle">Delete Active Directory User!</div>
        <div class="input-container ic2">
            <input id="acc-name" class="input" type="text" placeholder=" " name="accountName"/>
            <div class="cut cut-short"></div>
            <label for="acc-name" class="placeholder">Account Name</label>
          </div>      
        <input type="submit" class="submit" value="Submit">
      </div>
      </form>    
      <br>
</body>
d</html>