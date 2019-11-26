from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, SubmitField, BooleanField #To use stringField which import from the wtforms not the Flaskform
from wtforms.validators import DataRequired, Length, Email, EqualTo, ValidationError
from kms.authen import User

class LoginForm(FlaskForm):
	#set validation for username creation
	email = StringField("", 
			validators=[DataRequired(), Email(), Length(min=10, max=60)], render_kw={"placeholder": "Email:"})
	password = PasswordField("", 
			validators=[DataRequired(), Length(min=6, max=30)], render_kw={"placeholder": "Password:"})
	remember = BooleanField("Remeber me")
	submit = SubmitField("Login")

# class forgotForm(FlaskForm):
# 	#set validation for username creation
# 	email = StringField("", 
# 			validators=[DataRequired(), Email(), Length(min=10, max=60)], render_kw={"placeholder": "Email:"})
# 	leave = SubmitField("Back")
# 	send = SubmitField("Send Request")