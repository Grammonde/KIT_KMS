from kms import db, login_manager
from datetime import datetime
from flask_login import UserMixin

@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

class User (db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(120), unique=True, nullable=True)   
    password = db.Column(db.String(60), nullable=True)
    image_file = db.Column(db.String(20), nullable=False, default='default.jpg')
    #update = db.relationship('Post', backref='author', lazy=True)

    def __repr__ (self):
        return f"User('{self.email}, {self.password}')"

# class Post (db.Model):
#     tag_id = db.Column(db.Integer, primary_key=True)
#     date_time = db.Column(db.DateTime, default=datetime.utcnow, nullable=True)
#     key_num = db.Column(db.varchar(50), unique=True)

#     def __repr__ (self):
#         return f"Post('{self.tag_id}, {self.date_time}, {self.key_num}')"

# class key_info (db.Model):
#     key_num = db.Column(db.varchar(50), unique=True, db.ForeignKey())
#     status = db.Column(db.varchar(50), nullable=True)
#     room = db.Column(db.varchar(50), nullable=True)

#     def __repr__ (self):
#         return f"User('')"

# class user_info (user_info)
#     tag_Id = db.Column(db.Integer, unique=True, db.ForeignKey())
#     name = db.Column(db.varchar(50), unique=True, nullable=True)
#     position = db.Column(db.varchar(50), nullable=True)
#     kit_id = db.Column(db.varchar(50), nullable=True)
#     batch = db.Column(db.varchar(50), nullable=True)

#     def __repr__ (self):
#         return f"User('')"