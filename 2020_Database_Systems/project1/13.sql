SELECT P.name AS '상록시티 출신 트레이너가 가진 포켓몬의 이름', pid AS '포켓몬 ID'
FROM Trainer AS T, CatchedPokemon, Pokemon AS P
WHERE T.id = owner_id AND hometown = 'Sangnok City' AND pid = P.id 
ORDER BY pid;