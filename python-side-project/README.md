So much action in ~50 lines.

Simple python module to parse IMDB.

## Requirements:
* python 2 or 3

## Installation:

    sudo python2 setup.py install

or

    sudo python3 setup.py install
    
## Usage:

```python 
>>> from imdb import IMDB
>>> a = IMDB('http://www.imdb.com/title/tt1843866/?ref_=inth_ov_tt')
>>> a.parse()
>>> a.director
'Anthony Russo, Joe Russo'
>>> a.plot
'Steve Rogers struggles to embrace his role in the modern world and battles a new threat from old history: the Soviet agent known as the Winter Soldier.'
>>> a.title
'Captain America: The Winter Soldier'
>>> a.country
'USA'
>>> a.rating
'8.2/10 from 111,064 users'
>>> a.metascore
'Metascore: 70/100'  # side note: not every movie has a poster or metascore
>>> a.release_date
'4 April 2014'  # side note: visit yourimdblink/releaseinfo, to see all the release dates and understand why it's different than the one shown in imdb.
>>> a.genre
'Action', 'Adventure', 'Sci-Fi'
>>> a.poster
'http://ia.media-imdb.com/images/M/MV5BMzA2NDkwODAwM15BMl5BanBnXkFtZTgwODk5MTgzMTE@._V1_SY317_CR1,0,214,317_AL_.jpg'
>>> a.runtime
'136 min'
>>> a.casts
'Chris Evans, Samuel L. Jackson, Scarlett Johansson'
```
