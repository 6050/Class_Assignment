SELECT AVG(level) AS '고향이 상록시티인 트레이너들이 가지고 있는 전기타입 포켓몬 레벨의 평균'
FROM Trainer AS T, CatchedPokemon, Pokemon AS P
WHERE T.id = owner_id AND hometown = 'Sangnok City' AND pid = P.id AND type = 'Electric';