import os
basedir = os.path.abspath(os.path.dirname(__file__))

class Config(object):
    SECRET_KEY = os.environ.get('SECRET_KEY', 'you-will-never-guess')
    MYSQL_HOST = os.environ.get('MYSQL_HOST', 'localhost')
    MYSQL_DB = os.environ.get('MYSQL_DB', 'room_climate')
    MYSQL_USER = os.environ.get('MYSQL_USER', 'climate')
    MYSQL_PASS = os.environ.get('MYSQL_PASS', 'climate')
    SQLALCHEMY_DATABASE_URI = 'mysql://{user}:{password}@{host}/{db}'.format(
      user=MYSQL_USER,
      password=MYSQL_PASS,
      host=MYSQL_HOST,
      db=MYSQL_DB
    )
    #SQLALCHEMY_DATABASE_URI = 'sqlite:///' + os.path.join(basedir, 'climate.db')
    SQLALCHEMY_TRACK_MODIFICATIONS = False
