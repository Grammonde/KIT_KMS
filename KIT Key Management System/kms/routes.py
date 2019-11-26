from flask import render_template, url_for, flash, redirect, request
from kms import app, db, bcrypt
from kms.authen import User
from kms.loginform import LoginForm
from flask_login import login_user, current_user, logout_user, login_required


@app.route("/", methods=['GET','POST'])
def index():
    if current_user.is_authenticated:
         return redirect(url_for('log'))
    form = LoginForm()
    if form.validate_on_submit():
        user = User.query.filter_by(email=form.email.data).first()
        if user and bcrypt.check_password_hash(user.password, form.password.data):
            login_user(user, remember=form.remember.data)
            return redirect(url_for('log'))
        else:    
            flash('Login Unsuccessful. Please check email and password', 'danger')
    return render_template('login.html', title='Login', form=form)

@app.route("/login", methods=['GET','POST'])
def login():
    if current_user.is_authenticated:
         return redirect(url_for('log'))
    form = LoginForm()
    if form.validate_on_submit():
        user = User.query.filter_by(email=form.email.data).first()
        if user and bcrypt.check_password_hash(user.password, form.password.data):
            login_user(user, remember=form.remember.data)
            return redirect(url_for('log'))
        else:    
            flash('Login Unsuccessful. Please check email and password', 'danger')
    return render_template('login.html', title='Login', form=form)

# @app.route("/push_log", methods=['POST'])
# def push_log():
#     data = request.args.get('value1')
#     # code to store value1 to database
#     return 'Data: ' + str(data)

@app.route("/log", methods=['GET','POST'])
@login_required
def log(): 
	return render_template('log.html')

@app.route("/json", methods=['GET'])
@login_required
def json():
	return "JSON FILE IS NOT YET AVAILABLE"

@app.route("/forgot", methods=['GET','POST'])
def forgot():
    if current_user.is_authenticated:
         return redirect(url_for('log'))
    # form = forgotForm()
    # if form.validate_on_submit():
    #     if form.email.data == 'admin@blog.com':
    #         flash('Your email recovery is success', 'success')
    #         return redirect(url_for('login'))
    #     else:
    #         flash('Wrong email. Please check your email agin', 'warning')
    return render_template('forgot.html', title='Forgot')

@app.route("/logout")
def logout():
    logout_user()
    return redirect(url_for('login'))
