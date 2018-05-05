from app import db
from datetime import datetime


class Record(db.Model):
     __tablename__ = 'records'

     id = db.Column(db.Integer, primary_key=True)
     timestamp = db.Column(db.DateTime, index=True, default=datetime.utcnow)
     co2 = db.Column(db.Integer)
     hum = db.Column(db.Float)
     temp = db.Column(db.Float)


     def __repr__(self):
          return '<Record {}>'.format(self.timestamp)
