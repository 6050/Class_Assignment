SELECT COUNT(DISTINCT pid) AS '상록시티 출신 트레이너들이 가지고 있는 포켓몬 종류의 개수'
FROM Trainer AS T, CatchedPokemon
WHERE T.id = owner_id AND hometown = 'Sangnok City';