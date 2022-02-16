SELECT COUNT(DISTINCT type) AS '상록시티의 체육관 관장이 가지고 있는 포켓몬 타입의 개수'
FROM Trainer AS T, Gym, CatchedPokemon, Pokemon AS P
WHERE T.id = leader_id AND hometown = 'Sangnok City' AND T.id = owner_id AND pid = P.id;