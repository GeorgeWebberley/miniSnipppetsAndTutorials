from flask import render_template

from app import app

@app.route('/')
@app.route('/index')
def index():
    return render_template('index.html', title='Home', picture='1')


@app.route('/explore')
def explore():
    return render_template('explore.html', title='Explore', picture='2')


@app.route('/explore/dining')
def dining():
    return render_template('dining.html', title='Explore - Dining', picture='2')


@app.route('/explore/swimming')
def swimming():
    return render_template('swimming.html', title='Explore - Swimming', picture='2')


@app.route('/explore/bar')
def bar():
    return render_template('bar.html', title='Explore - Bar', picture='2')


@app.route('/explore/beaches')
def beaches():
    return render_template('beaches.html', title='Explore - Beaches', picture='2')


@app.route('/explore/villas')
def villas():
    return render_template('villas.html', title='Explore - Villas', picture='2')


@app.route('/explore/transport')
def transport():
    return render_template('transport.html', title='Explore - Transport', picture='2')


@app.route('/video')
def video():
    return render_template('video.html', title='Video', picture='1')
