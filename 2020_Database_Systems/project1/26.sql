SELECT name AS '잡힌 포켓몬 중 닉네임에 공백이 들어가는 포켓몬의 이름'
FROM CatchedPokemon, Pokemon AS P
WHERE pid = P.id AND nickname LIKE '% %'
ORDER BY name DESC;