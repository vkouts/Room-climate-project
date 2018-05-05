from app import db, admin
from .models import Record
from flask_admin.contrib.sqla import ModelView


admin.add_view(ModelView(Record, db.session))
