from app import app, db
from flask import render_template, url_for, redirect, request, abort, send_file
from app.models import Record


@app.route('/static/<path:path>')
def send_static(path):
    return send_from_directory(app.config['STATIC_FOLDER'], path)


@app.route('/', methods=['GET', 'POST'])
def index():
    records = Record.query.all()
    return render_template('index.html', records=records)
